#include "chef_thread_pool.h"

#ifdef __linux__
#include <sys/prctl.h>
#endif

namespace chef {

  thread_pool::thread_pool(int num_of_thread, const std::string &name)
    : num_of_thread_(num_of_thread)
    , thread_prefix_name_(name)
    , exit_flag_(false)
  {
  }

  thread_pool::~thread_pool() {
    exit_flag_ = true;
    cond_.notify_all();
    for (int i = 0; i < num_of_thread_; i++) {
      threads_[i]->join();
    }
  }

  void thread_pool::start() {
    for (int i = 0; i < num_of_thread_; i++) {
      thread_runned_events_.push_back(chef::make_shared<chef::wait_event_counter>());
      threads_.push_back(chef::make_shared<chef::thread>(
        chef::bind(&thread_pool::run_in_thread_, this, i)
      ));
      thread_runned_events_[i]->wait();
    }
  }

  void thread_pool::add(const task &t) {
    chef::unique_lock<chef::mutex> lock(mutex_);
    tasks_.push_back(t);
    cond_.notify_one();
  }

  uint64_t thread_pool::num_of_undone_task() {
    chef::lock_guard<chef::mutex> guard(mutex_);
    return tasks_.size();
  }

  void thread_pool::run_in_thread_(int index) {
#ifdef __linux__
    char thread_name[32] = {0};
    snprintf(thread_name, 31, "%s%d", thread_prefix_name_.c_str(), index+1);
    ::prctl(PR_SET_NAME, thread_name);
#endif
    thread_runned_events_[index]->notify();
    for (; !exit_flag_; ) {
      task t(take_());
      if (t) {
        t();
      }
    }
  }

  thread_pool::task thread_pool::take_() {
    chef::unique_lock<chef::mutex> lock(mutex_);
    for (; !exit_flag_ && tasks_.empty(); ) {
      cond_.wait(lock);
    }
    task t;
    if (!tasks_.empty()) {
      t = tasks_.front();
      tasks_.pop_front();
    }
    return t;
  }

} // namespace chef
