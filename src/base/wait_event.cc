#include "wait_event.h"

namespace chef {

  wait_event::wait_event() : done_(false) {}
  wait_event::~wait_event() {}

  void wait_event::notify() {
    std::unique_lock<std::mutex> lock(mutex_);
    done_ = true;
    cond_.notify_one();
  }

  void wait_event::wait() {
    std::unique_lock<std::mutex> lock(mutex_);
    while (!done_) {
      cond_.wait(lock);
    }
  }

  bool wait_event::wait_for(uint32_t timeout_ms) {
    if (timeout_ms == 0) {
      wait();
      return true;
    }

    std::unique_lock<std::mutex> lock(mutex_);

    return cond_.wait_for(lock, std::chrono::milliseconds(timeout_ms), [this]() {
      return this->done_;
    });
  }

} // namespace chef
