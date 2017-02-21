/**
 * @file   chef_task_thread.h/chef_task_thread.cc
 * @deps   chef_env.hpp/chef_noncopyable.hpp/chef_wait_event_counter.h
 *
 * @author
 *   chef <191201771@qq.com>
 *     -initial release xxxx-xx-xx
 *
 * @brief
 *   开启一个线程，可以往里面添加异步任务（支持延时任务）。
 *
 */

#ifndef _CHEF_BASE_TASK_THREAD_H_
#define _CHEF_BASE_TASK_THREAD_H_

#include "chef_wait_event_counter.h"
#include "chef_noncopyable.hpp"
#include "chef_env.hpp"
#include <stdint.h>
#include <string>
#include <deque>
#include <map>

namespace chef {

  class task_thread : public chef::noncopyable {
    public:
      typedef chef::function<void()> task;

      /// @NOTICE 如果选择 RELEASE_MODE_DO_SHOULD_DONE 或 RELEASE_MODE_DO_ALL_DONE，
      ///         内部依然会保证任务的执行线程体是task_thread内部开启的线程。
      ///         析构函数会等待应该执行的异步任务执行完再返回。
      enum release_mode {
        RELEASE_MODE_ASAP,           /// 析构时，未执行的任务全部丢弃。
        RELEASE_MODE_DO_SHOULD_DONE, /// 析构时，执行需要执行的任务——实时任务和已到定时时间的延时任务，未到定时时间的延时任务不执行。
        RELEASE_MODE_DO_ALL_DONE,    /// 析构时，执行所有任务——实时任务和所有延时任务，未到定时时间的延时任务也会提前执行。
      };

      /**
       * @param thread_name 线程名，linux平台下调用prctl设置，其他平台或<thread_name>为空则不设置~
       *
       */
      explicit task_thread(const std::string &thread_name=std::string(), release_mode rm=RELEASE_MODE_ASAP);

      ~task_thread();

      /**
       * 开启线程
       * 非阻塞函数
       *
       */
      void start();

      /**
       * 添加任务，任务按添加顺序执行。
       *
       * @param                 t 任务
       * @param defferred_time_ms 可指定延时多少毫秒后执行，如果为0，则尽快执行。
       *
       */
      void add(const task &t, int defferred_time_ms=0);

      /**
       * @return 还未执行的任务数量
       *
       */
      uint64_t num_of_undone_task();

      /**
       * @return 线程名
       */
      std::string thread_name() const { return name_; }

    private:
      /**
       * 新建线程的执行体loop
       *
       */
      void run_in_thread_();

      /**
       * 收集已到定时时间该执行的延时任务
       *
       * @param tasks 传出参数 将达到定时时间的延时任务插入<tasks>尾部
       *
       */
      void append_expired_tasks_(std::deque<task> &tasks);

      /**
       * 获取当前时间，单位毫秒
       *
       */
      uint64_t now_();

      /**
       * 等待线程结束
       *
       */
      void join_();

      /**
       * 执行<tasks>里的所有任务，并清空<tasks>
       *
       */
      void execute_tasks_(std::deque<task> &tasks);

      /**
       * @overload
       *
       * 执行<tasks>里的所有任务，并清空<tasks>
       *
       */
       void execute_tasks_(std::multimap<uint64_t, task> &tasks);

    private:
      std::string                    name_;
      release_mode                   release_mode_;
      bool                           exit_flag_;
      chef::shared_ptr<chef::thread> thread_;
      std::deque<task>               tasks_;
      std::multimap<uint64_t, task>  defferred_tasks_;
      chef::mutex                    mutex_;
      chef::condition_variable       cond_;
      chef::wait_event_counter       runned_event_;
  };

} // namespace chef

#endif // _CHEF_BASE_TASK_THREAD_H_
