/**
 * @file     chef_task_thread.h
 * @deps     chef_env.hpp | chef_wait_event_counter.hpp
 * @platform linux/macos/xxx
 *
 * @author
 *   chef <191201771@qq.com>
 *     - initial release xxxx-xx-xx
 *
 * @brief
 *   开启一个线程，可以往里面添加异步任务（支持延时任务）。
 *   任务可以是业务方的任意函数（通过bind/function实现）。
 *
 */

#ifndef _CHEF_BASE_TASK_THREAD_H_
#define _CHEF_BASE_TASK_THREAD_H_
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

      /// @NOTICE 如果选择 RELEASE_MODE_DO_SHOULD_DONE 或 RELEASE_MODE_DO_ALL_DONE，
      ///         内部依然会保证任务的执行线程体是task_thread内部开启的线程。
      ///         析构函数会等待应该执行的异步任务执行完再返回。
      enum release_mode {
        RELEASE_MODE_ASAP,           /// 析构时，未执行的任务全部丢弃。
        RELEASE_MODE_DO_SHOULD_DONE, /// 析构时，执行需要执行的任务——实时任务和已到定时时间的延时任务，未到定时时间的延时任务不执行。
        RELEASE_MODE_DO_ALL_DONE,    /// 析构时，执行所有任务——实时任务和所有延时任务，未到定时时间的延时任务也会提前执行。
      };

    public:
      /**
       * @param name 线程名，linux平台下调用prctl设置，其他平台或<name>为空则不设置~
       *
       */
      explicit task_thread(const std::string &name=std::string(), release_mode rm=RELEASE_MODE_ASAP);
      ~task_thread();

    private:
      task_thread(const task_thread &);
      task_thread &operator=(const task_thread &);

    public:
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
       * @param defferred_time_ms 可指定延时多少毫秒后执行，如果为0，则尽快执行，精度百毫秒级别。
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
       *
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

  }; // class task_thread

} // namespace chef





///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// @NOTICE 该分隔线以上部分为该模块的接口，分割线以下部分为对应的实现





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
  {
  }

  inline task_thread::~task_thread() {
    exit_flag_ = true;
    join_();
  }

  inline void task_thread::start() {
    thread_ = chef::make_shared<chef::thread>(chef::bind(&task_thread::run_in_thread_, this));
    runned_event_.wait();
  }

  inline void task_thread::join_() {
    if (thread_) {
      thread_->join();
    }
  }

  inline void task_thread::add(const task &t, int defferred_time_ms) {
    chef::lock_guard<chef::mutex> guard(mutex_);
    if (defferred_time_ms == 0) {
      tasks_.push_back(t);
    } else {
      defferred_tasks_.insert(std::pair<uint64_t, task>(now_() + defferred_time_ms, t));
    }
    cond_.notify_one();
  }

  inline uint64_t task_thread::num_of_undone_task() {
    chef::lock_guard<chef::mutex> guard(mutex_);
    return tasks_.size() + defferred_tasks_.size();
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
          //chef::cv_status cs = cond_.wait_for(lock, chef::chrono::milliseconds(100));
          //if (cs == chef::cv_status::timeout && !defferred_tasks_.empty()) {
          //    break;
          //}
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
//    auto iter = defferred_tasks_.begin();
    std::multimap<uint64_t, task>::iterator iter = defferred_tasks_.begin();
    for (; iter != defferred_tasks_.end(); iter++) {
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
      tasks.pop_front();
    }
  }

  inline void task_thread::execute_tasks_(std::multimap<uint64_t, task> &tasks) {
//    for (auto item : tasks) {
    std::multimap<uint64_t, task>::iterator iter = tasks.begin();
    for (; iter != tasks.end(); iter++) {
      iter->second();
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

#endif // _CHEF_BASE_TASK_THREAD_H_
