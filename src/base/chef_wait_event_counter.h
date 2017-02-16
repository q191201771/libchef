/**
 * @file   chef_wait_event_counter.h/chef_wait_event_counter.cc
 * @deps   chef_env.hpp/chef_noncopyable.hpp
 *
 * @author
 *   chef <191201771@qq.com>
 *     -initial release xxxx-xx-xx
 *
 * @brief
 *   阻塞等待N个事件发生。
 *
 */

#ifndef _CHEF_BASE_WAIT_EVENT_COUNTER_H_
#define _CHEF_BASE_WAIT_EVENT_COUNTER_H_

#include "chef_noncopyable.hpp"
#include "chef_env.hpp"

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
      const int                need_count_;
      chef::atomic<int>        count_;
      chef::mutex              mutex_;
      chef::condition_variable cond_;
  };

} // namespace chef

#endif // _CHEF_BASE_WAIT_EVENT_COUNTER_H_
