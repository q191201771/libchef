/**
 * @tag      v1.4.10
 * @file     chef_thread_pool.hpp
 * @deps     chef_env.hpp | chef_wait_event_counter.hpp
 * @platform linux | macos | xxx
 *
 * @author
 *   chef <191201771@qq.com>
 *     - initial release xxxx-xx-xx
 *
 * @brief
 *   线程池，池中的空闲线程抢占式执行加入的任务
 *   适用于任务不要求强顺序性执行的场景
 *
 */

#ifndef _CHEF_BASE_THREAD_POOL_HPP_
#define _CHEF_BASE_THREAD_POOL_HPP_
#pragma once

#include "chef_env.hpp"
#include "chef_wait_event_counter.hpp"
#include <string>
#include <vector>
#include <deque>

namespace chef {

  class thread_pool {
    public:
      typedef chef::function<void()> task;

    public:
      /**
       * @param num_of_thread 线程数
       * @param thread_prefix_name 线程前缀名，linux平台下调用prctl设置线程名，下标从1开始。其他平台或`thread_prefix_name`为空则不设置~
       *
       */
      explicit thread_pool(uint32_t num_of_thread, const std::string &thread_prefix_name=std::string());

      /**
       * -析构时会等待所有后台线程结束，即正在执行的任务需要执行完（肯定啦），未开始执行的任务就不执行了（出于速度释放资源考虑），
       *  所以可能会阻塞一会（依正执行任务的复杂度而定）~
       * -多说一句，用户不应该假定调用并执行完add后析构thread_pool，加入的task就一定会被执行，如果你是这种场景，建议你在
       *  task的尾巴上加个同步语句，比如使用chef_wait_event_counter~
       *
       */
      ~thread_pool();

    public:
      /// 开启后台线程池，非阻塞函数
      void start();

      /// 添加异步任务，非阻塞函数
      void add(const task &t);

      /// 返回还没执行完的任务数量
      uint64_t num_of_undone_task();

    private:
      void run_in_thread_(int index);
      task take_();

    private:
      thread_pool(const thread_pool &);
      thread_pool &operator=(const thread_pool &);

    private:
      typedef std::vector<chef::shared_ptr<chef::thread> >             thread_vector;
      typedef std::vector<chef::shared_ptr<chef::wait_event_counter> > wait_event_vector;

    private:
      uint32_t                 num_of_thread_;
      std::string              thread_prefix_name_;
      bool                     exit_flag_;
      thread_vector            threads_;
      wait_event_vector        thread_runned_events_;
      std::deque<task>         tasks_;
      chef::mutex              mutex_;
      chef::atomic<uint64_t>   num_of_undone_task_;
      chef::condition_variable cond_;
  };

} // namespace chef





///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// @NOTICE 该分隔线以上部分为该模块的接口，分割线以下部分为对应的实现





#include <stdio.h>
#include <assert.h>

#ifdef __linux__
#include <sys/prctl.h>
#endif

namespace chef {

  inline thread_pool::thread_pool(uint32_t num_of_thread, const std::string &thread_prefix_name)
    : num_of_thread_(num_of_thread)
    , thread_prefix_name_(thread_prefix_name)
    , exit_flag_(false)
    , num_of_undone_task_(0)
  {
    assert(num_of_thread > 0);
  }

  inline thread_pool::~thread_pool() {
    exit_flag_ = true;
    cond_.notify_all();
    for (uint32_t i = 0; i < num_of_thread_; i++) {
      threads_[i]->join();
    }
  }

  inline void thread_pool::start() {
    for (uint32_t i = 0; i < num_of_thread_; i++) {
      thread_runned_events_.push_back(chef::make_shared<chef::wait_event_counter>());
      threads_.push_back(chef::make_shared<chef::thread>(chef::bind(&thread_pool::run_in_thread_, this, i)));
      thread_runned_events_[i]->wait();
    }
  }

  inline void thread_pool::add(const task &t) {
    num_of_undone_task_++;
    chef::unique_lock<chef::mutex> lock(mutex_);
    tasks_.push_back(t);
    cond_.notify_one();
  }

  inline uint64_t thread_pool::num_of_undone_task() {
    return num_of_undone_task_.load();
  }

  inline void thread_pool::run_in_thread_(int index) {
#ifdef __linux__
    if (!thread_prefix_name_.empty()) {
      char thread_name[32] = {0};
      snprintf(thread_name, 31, "%s%d", thread_prefix_name_.c_str(), index+1);
      ::prctl(PR_SET_NAME, thread_name);
    }
#endif
    thread_runned_events_[index]->notify();
    for (; !exit_flag_; ) {
      task t(take_());
      if (t) {
        t();
        num_of_undone_task_--;
      }
    }
  }

  inline thread_pool::task thread_pool::take_() {
    chef::unique_lock<chef::mutex> lock(mutex_);
    for (; !exit_flag_ && tasks_.empty(); ) {
      cond_.wait(lock);
    }
    task t;
    if (!tasks_.empty()) {
      t = tasks_.front();
      tasks_.pop_front();
    }
    return t;
  }

} // namespace chef

#endif // _CHEF_BASE_THREAD_POOL_HPP_
