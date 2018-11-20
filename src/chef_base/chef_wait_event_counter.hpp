/**
 * @tag      v1.5.15
 * @file     chef_wait_event_counter.hpp
 * @deps     chef_env.hpp
 * @platform linux | macos | xxx
 *
 * @author
 *   chef <191201771@qq.com>
 *     - initial release xxxx-xx-xx
 *
 * @brief
 *   阻塞等待1~N次事件发生
 *   也可选择设置超时时间，超时后不再阻塞等待
 *   使用者不用关心条件变量、锁等线程同步实现的细节（例如事件发送与接收的时序，原子计数，虚假唤醒等）
 *
 */

#ifndef _CHEF_BASE_WAIT_EVENT_COUNTER_HPP_
#define _CHEF_BASE_WAIT_EVENT_COUNTER_HPP_
#pragma once

#include "chef_env.hpp"

namespace chef {

  class wait_event_counter {
    public:
      /**
       * @function 构造函数
       * @param nc 需要notify多少次，wait才能通过
       *
       */
      explicit wait_event_counter(int nc=1);
      ~wait_event_counter();

    public:
      /// 非阻塞，notify调用发生在wait执行前或wait中，都有效~
      void notify();

      /// 阻塞直到构造函数中指定的<need_count_>次notify已发生。
      void wait();

      /**
       * 阻塞直到构造函数中指定的<need_count_>次notify已发生或超时。
       *
       * @param timeout_ms 超时时间，单位毫秒，如果设置为0，则无超时机制。
       *
       * @return
       *   true  通知达成
       *   false 超时达成
       *
       */
      bool wait_for(uint32_t timeout_ms);

    public:
      int need_count() const { return need_count_; }
      int counted() const { return counted_; }

    private:
      bool check_();

    private:
      wait_event_counter(const wait_event_counter &);
      wait_event_counter operator=(const wait_event_counter &);

    private:
      const int                need_count_;
      chef::atomic<int>        counted_;
      chef::mutex              mutex_;
      chef::condition_variable cond_;
  };

} // namespace chef





///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// @NOTICE 该分隔线以上部分为该模块的接口，分割线以下部分为对应的实现





namespace chef {

  inline wait_event_counter::wait_event_counter(int nc)
    : need_count_(nc)
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

#endif // _CHEF_BASE_WAIT_EVENT_COUNTER_HPP_
