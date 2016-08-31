/**
 * @file   thread_pool.h/thread_pool.cc
 * @deps   nope
 *
 * @author
 *   chef <191201771@qq.com>
 *     -initial release 2016-08-31
 *
 * @brief
 *   线程池，池中的线程抢占式执行丢入其中的任务。
 *
 *   同chef::task_thread的使用场景一样，thread_pool对象的生命周期一般持续到程序结束。
 *   如果调用析构函数，应该尽快清理退出，不保证所有剩余任务执行完再退出。
 *
 */

#ifndef _CHEF_BASE_THREAD_POOL_H_
#define _CHEF_BASE_THREAD_POOL_H_

#include "wait_event.h"
#include "noncopyable.hpp"
#include <deque>
#include <string>
#include <vector>
#include <mutex>
#include <thread>
#include <functional>

namespace chef {

  class thread_pool : public chef::noncopyable {
    public:
      typedef std::function<void()> task;

      explicit thread_pool(int num_of_thread, const std::string &name = std::string("thread_pool"));
      ~thread_pool();

      ///
      void start();

      ///
      void add(const task &t);

      ///
      int num_of_undone_task();

    private:
      void run_in_thread(int index);
      task take();

    private:
      typedef std::vector<std::shared_ptr<std::thread> >      thread_vector;
      typedef std::vector<std::shared_ptr<chef::wait_event> > wait_event_vector;

      int                     num_of_thread_;
      std::string             thread_prefix_name_;
      bool                    exit_flag_;
      thread_vector           threads_;
      wait_event_vector       thread_runned_events_;
      std::deque<task>        tasks_;
      std::mutex              mutex_;
      std::condition_variable cond_;
  };

} // namespace chef

#endif
