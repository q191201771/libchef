/**
 * @file   chef_thread_pool.hpp[_impl]
 * @deps   chef_env.hpp | chef_noncopyable.hpp | chef_wait_event_counter.hpp[_impl]
 * @platform linux/macos/xxx
 *
 * @author
 *   chef <191201771@qq.com>
 *     -initial release xxxx-xx-xx
 *
 * @brief
 *   线程池，池中的空闲线程抢占式执行丢入其中的任务。
 *
 */

#ifndef _CHEF_BASE_THREAD_POOL_H_
#define _CHEF_BASE_THREAD_POOL_H_
#pragma once

#include "chef_wait_event_counter.hpp"
#include "chef_noncopyable.hpp"
#include "chef_env.hpp"
#include <string>
#include <vector>
#include <deque>

namespace chef {

  class thread_pool : public chef::noncopyable {
    public:
      typedef chef::function<void()> task;

      /**b
       * @param num_of_thread 线程数
       * @param thread_prefix_name 线程前缀名，linux平台下调用prctl设置线程名，下标从1开始。其他平台或`thread_prefix_name`为空则不设置~
       *
       */
      explicit thread_pool(int num_of_thread, const std::string &thread_prefix_name=std::string());

      /**
       * -析构时会等待所有后台线程结束，即正在执行的任务需要执行完（肯定啦），未开始执行的任务就不执行了（出于速度释放资源考虑），
       *  所以可能会阻塞一会（依正执行任务的复杂度而定）~
       * -多说一句，用户不应该假定调用并执行完add后析构thread_pool，加入的task就一定会被执行，如果你是这种场景，建议你在
       *  task的尾巴上加个同步语句，比如使用chef_wait_event_counter~
       *
       */
      ~thread_pool();

      /// 开启后台线程池，非阻塞函数
      void start();

      /// 添加异步任务，非阻塞函数
      void add(const task &t);

      /// 返回已添加还没来得及执行完的任务数量
      uint64_t num_of_undone_task();

    private:
      void run_in_thread_(int index);
      task take_();

    private:
      typedef std::vector<chef::shared_ptr<chef::thread> >             thread_vector;
      typedef std::vector<chef::shared_ptr<chef::wait_event_counter> > wait_event_vector;

      int                      num_of_thread_;
      std::string              thread_prefix_name_;
      bool                     exit_flag_;
      thread_vector            threads_;
      wait_event_vector        thread_runned_events_;
      std::deque<task>         tasks_;
      chef::mutex              mutex_;
      chef::condition_variable cond_;
  };

} // namespace chef

#include "chef_thread_pool_impl.hpp"

#endif
