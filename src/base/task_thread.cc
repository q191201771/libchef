#include "task_thread.h"
#include <sys/prctl.h>

namespace chef {

  task_thread::task_thread(const std::string &thread_name)
    : name_(thread_name)
    , exit_flag_(false)
    , thread_(NULL)
  {
  }

  task_thread::~task_thread() {
    exit_flag_ = true;
    join();
  }

  void task_thread::start() {
    thread_ = std::make_shared<std::thread>(std::bind(
      &task_thread::run_in_thread,
      this,
      name_
    ));
    runned_event_.wait();
  }

  void task_thread::join() {
    if (thread_) {
      thread_->join();
    }
  }

  void task_thread::add(const task &t, int defferred_time_ms) {
    std::lock_guard<std::mutex> guard(mutex_);
    if (defferred_time_ms == 0) {
      tasks_.push_back(t);
    } else {
      defferred_tasks_.insert(std::pair<uint64_t, task>(now() + defferred_time_ms, t));
    }
  }

  int task_thread::num_of_undone_task() {
    std::lock_guard<std::mutex> guard(mutex_);
    return tasks_.size() + defferred_tasks_.size();
  }

  void task_thread::run_in_thread(const std::string &name) {
    prctl(PR_SET_NAME, name.c_str());

    runned_event_.notify();

    std::deque<task> collect_tasks;
    while(!exit_flag_) {
      {
        std::unique_lock<std::mutex> lock(mutex_);
        cond_.wait_for(lock, std::chrono::milliseconds(100));
        if (!tasks_.empty()) {
          collect_tasks.swap(tasks_);
        }
        append_expired_task(collect_tasks);
      }
      while(!collect_tasks.empty()) {
        task t = collect_tasks.front();
        collect_tasks.pop_front();
        t();
      }
    }
  }

  void task_thread::append_expired_task(std::deque<task> &tasks) {
    if (defferred_tasks_.empty()) {
      return;
    }
    uint64_t now_ms = now();
    auto iter = defferred_tasks_.begin();
    for (; iter != defferred_tasks_.end(); iter++) {
      if (iter->first > now_ms) {
        break;
      }
      tasks.push_back(iter->second);
    }
    defferred_tasks_.erase(defferred_tasks_.begin(), iter);
  }

  uint64_t task_thread::now() {
    struct timespec ts;
    clock_gettime(CLOCK_MONOTONIC, &ts);
    return ts.tv_sec * 1000 + ts.tv_nsec / 1000000;
  }

} // namespace chef
