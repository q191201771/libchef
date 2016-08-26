/**
 * @file   wait_event.h/wait_event.cc
 * @deps   nope
 *
 * @author
 *   chef <191201771@qq.com>
 *     -initial release xxxx-xx-xx
 *
 * @brief
 *   对一次性的用于同步两个并行操作的condition_variable的封装，使用者不用关心cv，mutex的细节。
 *
 */

#ifndef _CHEF_BASE_WAIT_EVENT_H_
#define _CHEF_BASE_WAIT_EVENT_H_

#include "noncopyable.hpp"
#include <chrono>
#include <mutex>
#include <condition_variable>

namespace chef {

  class wait_event : public noncopyable {
    public:
      wait_event();
      ~wait_event();

      ///
      void notify();

      /**
       * 即使notify调用发生在wait前,wait同样可以被通知到.
       *
       */
      void wait();

      /**
       * 等待被通知或超时
       *
       * @param timeout_ms 超时时间,单位毫秒,如果设置0,则无超时机制.
       *
       * @return
       *   true  notified
       *   false timeout
       *
       */
      bool wait_for(uint32_t timeout_ms);

    private:
      std::mutex mutex_;
      std::condition_variable cond_;
      bool done_;
  };

} // namespace chef

#endif // _CHEF_BASE_WAIT_EVENT_H_
