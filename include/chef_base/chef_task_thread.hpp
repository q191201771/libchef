/**
 * @tag      v1.5.16
 * @file     chef_task_thread.h
 * @deps     chef_env.hpp | chef_wait_event_counter.hpp
 * @platform linux | macos | xxx
 *
 * @author
 *   chef <191201771@qq.com>
 *     - initial release xxxx-xx-xx
 *
 * @brief
 *   开启一个线程，可以往里面持续添加异步任务，任务串行执行，且执行顺序与添加顺序一致
 *   支持添加延时任务
 *
 *   任务可以是业务方的任意函数（通过bind/function实现）
 *   例如
 *   ```
 *   void func1() {}
 *
 *   void func2(int a) {}
 *
 *   class Foo {
 *   public:
 *     void func3(int b) {}
 *   };
 *
 *   chef::task_thread tt;
 *   tt.start();
 *   tt.add(chef::bind(&func1));
 *   tt.add(chef::bind(&fun2, 10));
 *   Foo foo;
 *   tt.add(chef::bind(&Foo::func3, &foo, 20));
 *   ```
 *   注意，如果函数参数是指针类型，那么需确保在任务线程执行该任务前，指针指向内容依然有效
 *
 */

#ifndef _CHEF_BASE_TASK_THREAD_HPP_
#define _CHEF_BASE_TASK_THREAD_HPP_
#pragma once

#include "chef_env.hpp"
#include "chef_wait_event_counter.hpp"
#include <stdint.h>
#include <string>
#include <deque>
#include <map>

namespace chef {

  class task_thread {
    public:
      typedef chef::function<void()> task;

      /**
       * @NOTICE 如果选择 RELEASE_MODE_DO_SHOULD_DONE 或 RELEASE_MODE_DO_ALL_DONE，
       *         内部依然会保证任务的执行线程体是task_thread内部开启的线程。
       *         析构函数会等待应该执行的异步任务执行完再返回。
       *
       */
      enum release_mode {
        RELEASE_MODE_ASAP,           /// 停止时，未执行的任务全部丢弃。
        RELEASE_MODE_DO_SHOULD_DONE, /// 停止时，执行需要执行的任务——实时任务和已到定时时间的延时任务，未到定时时间的延时任务不执行。
        RELEASE_MODE_DO_ALL_DONE,    /// 停止时，执行所有任务——实时任务和所有延时任务，未到定时时间的延时任务也会提前执行。
      };

    public:
      /**
       * @param name 线程名，linux平台下调用prctl设置，其他平台或<name>为空则不设置~
       *
       */
      explicit task_thread(const std::string &name=std::string(), release_mode rm=RELEASE_MODE_ASAP);
      ~task_thread();

    public:
      /// 开启后台线程，非阻塞函数
      void start();

      /**
       * 添加异步任务，非阻塞函数
       *
       * @param t 任务
       * @param defferred_time_ms 如果为0，则按添加顺序尽快执行；如果不为0，则延时指定毫秒后执行，精度百毫秒级别。
       *
       */
      void add(const task &t, int defferred_time_ms=0);

      /// 关闭后台线程，并阻塞等待线程结束
      void stop_and_join();

      /// 返回还没执行完的任务数量
      uint64_t num_of_undone_task();

      /// 返回线程名
      std::string thread_name() const { return name_; }

    private:
      /// 新建线程的执行体loop
      void run_in_thread_();

      /// 收集已到定时时间该执行的延时任务
      void append_expired_tasks_(std::deque<task> &tasks);

      /// 获取当前时间，单位毫秒
      uint64_t now_();

      /// 执行<tasks>里的所有任务，并清空<tasks>
      void execute_tasks_(std::deque<task> &tasks);

      /// @overload
      void execute_tasks_(std::multimap<uint64_t, task> &tasks);

    private:
      task_thread(const task_thread &);
      task_thread &operator=(const task_thread &);

    private:
      std::string                    name_;
      release_mode                   release_mode_;
      chef::atomic<bool>             exit_flag_;
      chef::shared_ptr<chef::thread> thread_;
      std::deque<task>               tasks_;
      std::multimap<uint64_t, task>  defferred_tasks_;
      chef::mutex                    mutex_;
      chef::atomic<uint64_t>         num_of_undone_task_;
      chef::condition_variable       cond_;
      chef::wait_event_counter       runned_event_;

  }; // class task_thread

} // namespace chef





///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// @NOTICE 该分隔线以上部分为该模块的接口，分割线以下部分为对应的实现





#include <stdio.h>
#include <time.h>
#include <sys/time.h>
#ifdef __linux__
#include <sys/prctl.h>
#endif

namespace chef {

  inline task_thread::task_thread(const std::string &name, release_mode rm)
    : name_(name)
    , release_mode_(rm)
    , exit_flag_(false)
    , num_of_undone_task_(0)
  {
  }

  inline task_thread::~task_thread() {
    stop_and_join();
  }

  inline void task_thread::start() {
    thread_ = chef::make_shared<chef::thread>(chef::bind(&task_thread::run_in_thread_, this));
    runned_event_.wait();
  }

  inline void task_thread::stop_and_join() {
    exit_flag_ = true;
    if (thread_) {
      thread_->join();
      thread_.reset();
    }
  }

  inline void task_thread::add(const task &t, int defferred_time_ms) {
    num_of_undone_task_++;
    chef::lock_guard<chef::mutex> guard(mutex_);
    if (defferred_time_ms == 0) {
      tasks_.push_back(t);
    } else {
      defferred_tasks_.insert(std::pair<uint64_t, task>(now_() + defferred_time_ms, t));
    }
    cond_.notify_one();
  }

  inline uint64_t task_thread::num_of_undone_task() {
    return num_of_undone_task_.load();
  }

  inline void task_thread::run_in_thread_() {
#ifdef __linux__
    if (!name_.empty()) {
      prctl(PR_SET_NAME, name_.c_str());
    }
#endif

    runned_event_.notify();

    std::deque<task> collect_tasks;
    for (; ; ) {
      { /// enter lock scope
        chef::unique_lock<chef::mutex> lock(mutex_);
        for (; !exit_flag_ && tasks_.empty(); ) {
          cond_.wait_for(lock, chef::chrono::milliseconds(100));
          if (!defferred_tasks_.empty()) {
              break;
          }
        }
        /// 收集需要立即执行的任务
        if (!tasks_.empty()) {
          collect_tasks.swap(tasks_);
        }
        /// 收集已到定时时间的延时任务
        if (!defferred_tasks_.empty()) {
          append_expired_tasks_(collect_tasks);
        }
        /// @NOTICE 把这个判断放在两次收集任务的下面，便于退出时通过release_mode_判断执行需要执行的任务
        if (exit_flag_) {
            break;
        }
      } /// leave lock scope
      execute_tasks_(collect_tasks);
    }

    chef::lock_guard<chef::mutex> lock(mutex_);
    switch (release_mode_) {
    case RELEASE_MODE_ASAP:
      break;
    case RELEASE_MODE_DO_SHOULD_DONE:
      execute_tasks_(collect_tasks);
      break;
    case RELEASE_MODE_DO_ALL_DONE:
      execute_tasks_(collect_tasks);
      execute_tasks_(defferred_tasks_);
      break;
    }
  }

  inline void task_thread::append_expired_tasks_(std::deque<task> &tasks) {
    if (defferred_tasks_.empty()) {
      return;
    }

    uint64_t now_ms = now_();

    std::multimap<uint64_t, task>::iterator iter = defferred_tasks_.begin();
    for (; iter != defferred_tasks_.end(); ++iter) {
      if (iter->first > now_ms) {
        break;
      }
      tasks.push_back(iter->second);
    }
    defferred_tasks_.erase(defferred_tasks_.begin(), iter);
  }

  inline void task_thread::execute_tasks_(std::deque<task> &tasks) {
    for (; !tasks.empty(); ) {
      tasks.front()();
      num_of_undone_task_--;
      tasks.pop_front();
    }
  }

  inline void task_thread::execute_tasks_(std::multimap<uint64_t, task> &tasks) {
//    for (auto item : tasks) {
    std::multimap<uint64_t, task>::iterator iter = tasks.begin();
    for (; iter != tasks.end(); ++iter) {
      iter->second();
      num_of_undone_task_--;
    }
    tasks.clear();
  }

  inline uint64_t task_thread::now_() {
    struct timespec ts;
#if defined(CLOCK_REALTIME) && !defined(__MACH__)
    clock_gettime(CLOCK_MONOTONIC, &ts);
#else
    {
      struct timeval tv;
      gettimeofday(&tv, NULL);
      ts.tv_sec = tv.tv_sec;
      ts.tv_nsec = tv.tv_usec * 1000;
    }
#endif
    return ts.tv_sec * 1000 + ts.tv_nsec / 1000000;
  }

} // namespace chef

#endif // _CHEF_BASE_TASK_THREAD_HPP_
