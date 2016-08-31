/**
 * @file   task_thread.h/task_thread.cc
 * @deps   nope
 *
 * @author
 *   chef <191201771@qq.com>
 *     -initial release 2016-08-31
 *
 * @brief
 *   开启一个线程，可以往里面添加异步任务。
 *
 *   使用task_thread的场景一般是线程一直开着，作为任务的执行环境，生命周期与程序一致，
 *   如果析构task_thread，内部会尽快关闭线程，避免析构函数阻塞，
 *   所以不保证已添加但未执行的剩余任务全部执行完。
 *
 */

#ifndef _CHEF_BASE_TASK_THREAD_H_
#define _CHEF_BASE_TASK_THREAD_H_

#include "noncopyable.hpp"
#include "wait_event.h"
#include <stdint.h>
#include <string>
#include <deque>
#include <map>
#include <mutex>
#include <thread>
#include <memory>
#include <functional>

namespace chef {

  class task_thread : public chef::noncopyable {
    public:
      typedef std::function<void()> task;

      explicit task_thread(const std::string &thread_name = std::string("task thread"));

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
       * @param t                 任务
       * @param defferred_time_ms 可指定延时多少毫秒后执行，如果为0，则尽快执行。
       *
       */
      void add(const task &t, int defferred_time_ms = 0);

      /**
       * 未执行的任务
       * 非阻塞函数
       *
       */
      int num_of_undone_task();

    private:
      /**
       * 新建线程的loop
       *
       */
      void run_in_thread(const std::string &name);

      /**
       * 收集已经到时间该执行延时任务
       *
       * @param tasks 传出参数 将该执行的延时任务插入[tasks]尾部
       *
       */
      void append_expired_task(std::deque<task> &tasks);

      /**
       * 获取当前时间，单位毫秒
       *
       */
      uint64_t now();

      /**
       * 等待线程结束
       *
       */
      void join();

    private:
      std::string                   name_;
      bool                          exit_flag_;
      std::shared_ptr<std::thread>  thread_;
      std::deque<task>              tasks_;
      std::multimap<uint64_t, task> defferred_tasks_;
      std::mutex                    mutex_;
      std::condition_variable       cond_;
      chef::wait_event              runned_event_;
  };

} // namespace chef

#endif // _CHEF_BASE_TASK_THREAD_H_
