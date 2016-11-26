/**
 * @file   wait_event_counter.h/wait_event_counter.cc
 * @deps   nope
 *
 * @author
 *   chef <191201771@qq.com>
 *     -initial release 2016-11-26
 *
 * @brief
 *   阻塞等待N个事件发生。
 *
 */

#ifndef _CHEF_BASE_WAIT_EVENT_COUNTER_H_
#define _CHEF_BASE_WAIT_EVENT_COUNTER_H_

#include "noncopyable.hpp"
#include <mutex>
#include <atomic>
#include <chrono>
#include <condition_variable>

namespace chef {

  class wait_event_counter : public noncopyable {
    public:
      /**
       * @param count 需要notify多少次，wait才能通过
       */
      wait_event_counter(int need_count = 1);

      ~wait_event_counter();

      /// notify可发生在wait前，也可发生在wait中。
      void notify();

      /// 阻塞直到构造函数中指定的[count]次notify已发生。
      void wait();

    private:
      const int               need_count_;
      std::atomic<int>        count_;
      std::mutex              mutex_;
      std::condition_variable cond_;
  };

} // namespace chef

#endif // _CHEF_BASE_WAIT_EVENT_COUNTER_H_
