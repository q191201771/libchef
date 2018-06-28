/**
 * @file     chef_env.hpp
 * @deps     libboost or c++11
 * @platform linux/macos/xxx
 *
 * @author
 *   chef <191201771@qq.com>
 *     -initial release xxxx-xx-xx
 *
 * @brief  由于一些开发环境不能使用c++11，做一个boost和c++11的选择
 *
 */

#ifndef _CHEF_BASE_ENV_HPP_
#define _CHEF_BASE_ENV_HPP_
#pragma once

// #define CHEF_USE_BOOST

#ifdef CHEF_USE_BOOST
  #include <boost/thread.hpp>
  #include <boost/atomic.hpp>
  #include <boost/smart_ptr.hpp>
  #include <boost/functional.hpp>
  #include <boost/chrono.hpp>
  namespace chef_env_keeper = boost;
#else
  #include <mutex>
  #include <thread>
  #include <condition_variable>
  #include <atomic>
  #include <memory>
  #include <functional>
  #include <chrono>
  namespace chef_env_keeper = std;
#endif

namespace chef {
  using chef_env_keeper::thread;
  using chef_env_keeper::mutex;
  using chef_env_keeper::lock_guard;
  using chef_env_keeper::unique_lock;
  using chef_env_keeper::condition_variable;
  typedef chef_env_keeper::cv_status cv_status;
  namespace this_thread = chef_env_keeper::this_thread;

  using chef_env_keeper::atomic;

  using chef_env_keeper::shared_ptr;
  using chef_env_keeper::make_shared;
  using chef_env_keeper::enable_shared_from_this;

  using chef_env_keeper::bind;
  using chef_env_keeper::function;

  namespace chrono = chef_env_keeper::chrono;

} // namespace chef

#endif /// _CHEF_BASE_ENV_HPP_
