#include "chef_task_thread.h"

#ifdef __linux__
#include <sys/prctl.h>
#endif

namespace chef {

  task_thread::task_thread(const std::string &thread_name, release_mode rm)
    : name_(thread_name)
    , release_mode_(rm)
    , exit_flag_(false)
  {
  }

  task_thread::~task_thread() {
    exit_flag_ = true;
    join_();
  }

  void task_thread::start() {
    thread_ = chef::make_shared<chef::thread>(chef::bind(&task_thread::run_in_thread_, this));
    runned_event_.wait();
  }

  void task_thread::join_() {
    if (thread_) {
      thread_->join();
    }
  }

  void task_thread::add(const task &t, int defferred_time_ms) {
    chef::lock_guard<chef::mutex> guard(mutex_);
    if (defferred_time_ms == 0) {
      tasks_.push_back(t);
    } else {
      defferred_tasks_.insert(std::pair<uint64_t, task>(now_() + defferred_time_ms, t));
    }
  }

  uint64_t task_thread::num_of_undone_task() {
    chef::lock_guard<chef::mutex> guard(mutex_);
    return tasks_.size() + defferred_tasks_.size();
  }

  void task_thread::run_in_thread_() {
#ifdef __linux__
    prctl(PR_SET_NAME, name_.c_str());
#endif

    runned_event_.notify();

    std::deque<task> collect_tasks;
    for (; ; ) {
      { /// enter lock scope
        chef::unique_lock<chef::mutex> lock(mutex_);
        for (; !exit_flag_ && tasks_.empty() && defferred_tasks_.empty(); ) {
          cond_.wait_for(lock, chef::chrono::milliseconds(100));
        }
        /// 收集需要立即执行的任务
        if (!tasks_.empty()) {
          collect_tasks.swap(tasks_);
        }
        /// 收集已到定时时间的延时任务
        if (!defferred_tasks_.empty()) {
          append_expired_tasks_(collect_tasks);
        }
        /// @NOTICE 把这个判断放在两次收集任务的下面，便于退出时通过release_mode_判断执行需要执行的任务
        if (exit_flag_) {
            break;
        }
      } /// leave lock scope
      execute_tasks_(collect_tasks);
    }

    chef::lock_guard<chef::mutex> lock(mutex_);
    switch (release_mode_) {
    case RELEASE_MODE_ASAP:
      break;
    case RELEASE_MODE_DO_SHOULD_DONE:
      execute_tasks_(collect_tasks);
      break;
    case RELEASE_MODE_DO_ALL_DONE:
      execute_tasks_(collect_tasks);
      execute_tasks_(defferred_tasks_);
      break;
    }
  }

  void task_thread::append_expired_tasks_(std::deque<task> &tasks) {
    if (defferred_tasks_.empty()) {
      return;
    }

    uint64_t now_ms = now_();
//    auto iter = defferred_tasks_.begin();
    std::multimap<uint64_t, task>::iterator iter = defferred_tasks_.begin();
    for (; iter != defferred_tasks_.end(); iter++) {
      if (iter->first > now_ms) {
        break;
      }
      tasks.push_back(iter->second);
    }
    defferred_tasks_.erase(defferred_tasks_.begin(), iter);
  }

  void task_thread::execute_tasks_(std::deque<task> &tasks) {
    for (; !tasks.empty(); ) {
      tasks.front()();
      tasks.pop_front();
    }
  }

  void task_thread::execute_tasks_(std::multimap<uint64_t, task> &tasks) {
//    for (auto item : tasks) {
    std::multimap<uint64_t, task>::iterator iter = tasks.begin();
    for (; iter != tasks.end(); iter++) {
      iter->second();
    }
    tasks.clear();
  }

  uint64_t task_thread::now_() {
    struct timespec ts;
    clock_gettime(CLOCK_MONOTONIC, &ts);
    return ts.tv_sec * 1000 + ts.tv_nsec / 1000000;
  }

} // namespace chef
