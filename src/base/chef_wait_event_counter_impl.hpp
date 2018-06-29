
namespace chef {

  inline wait_event_counter::wait_event_counter(int need_count)
    : need_count_(need_count)
    , counted_(0)
  {
  }

  inline wait_event_counter::~wait_event_counter() {}

  inline void wait_event_counter::notify() {
    chef::unique_lock<chef::mutex> lock(mutex_);
    counted_++;
    cond_.notify_one();
  }

  inline void wait_event_counter::wait() {
    chef::unique_lock<chef::mutex> lock(mutex_);
    for (; counted_.load() < need_count_; ) {
      cond_.wait(lock);
    }
  }

  inline bool wait_event_counter::wait_for(uint32_t timeout_ms) {
    if (timeout_ms == 0) {
      wait();
      return true;
    }

    chef::unique_lock<chef::mutex> lock(mutex_);
    return cond_.wait_for(lock,
                          chef::chrono::milliseconds(timeout_ms),
    //                      [this]() {
    //                        return this->counted_.load() >= need_count_;
    //                      });
                          chef::bind(&wait_event_counter::check_, this));
  }

  inline bool wait_event_counter::check_() { return this->counted_.load() >= need_count_; }

} // namespace chef
