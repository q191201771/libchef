#include "chef_wait_event.h"

namespace chef {

  wait_event::wait_event() : done_(false) {}
  wait_event::~wait_event() {}

  void wait_event::notify() {
    chef::unique_lock<chef::mutex> lock(mutex_);
    done_ = true;
    cond_.notify_one();
  }

  void wait_event::wait() {
    chef::unique_lock<chef::mutex> lock(mutex_);
    for (; !done_; ) {
      cond_.wait(lock);
    }
  }

  bool wait_event::wait_for(uint32_t timeout_ms) {
    if (timeout_ms == 0) {
      wait();
      return true;
    }

    chef::unique_lock<chef::mutex> lock(mutex_);

    return cond_.wait_for(lock, chef::chrono::milliseconds(timeout_ms), [this]() {
      return this->done_;
    });
  }

} // namespace chef
