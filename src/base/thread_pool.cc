#include "thread_pool.h"
#include <sys/prctl.h>

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
      thread_runned_events_.push_back(std::make_shared<chef::wait_event>());
      threads_.push_back(std::make_shared<std::thread>(
        std::bind(&thread_pool::run_in_thread, this, i)
      ));
      thread_runned_events_[i]->wait();
    }
  }

  void thread_pool::add(const task &t) {
    std::unique_lock<std::mutex> lock(mutex_);
    tasks_.push_back(t);
    cond_.notify_one();
  }

  int thread_pool::num_of_undone_task() {
    std::lock_guard<std::mutex> guard(mutex_);
    return tasks_.size();
  }

  void thread_pool::run_in_thread(int index) {
    char thread_name[32] = {0};
    snprintf(thread_name, 31, "%s%d", thread_prefix_name_.c_str(), index+1);
    ::prctl(PR_SET_NAME, thread_name);
    thread_runned_events_[index]->notify();
    while(!exit_flag_) {
      task t(take());
      if (t) {
        t();
      }
    }
  }

  thread_pool::task thread_pool::take() {
    std::unique_lock<std::mutex> lock(mutex_);
    while(!exit_flag_ && tasks_.empty()) {
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
