/**
 * @tag      v1.5.15
 * @file     chef_env.hpp
 * @deps     c++11 or libboost
 * @platform linux | macos | xxx
 *
 * @author
 *   chef <191201771@qq.com>
 *     - initial release xxxx-xx-xx
 *
 * @brief
 *   c++11和libboost功能相同部分的wrapper
 *   通过增加一层接入层，使上层代码仅需通过一个宏开关就可以自由切换使用c++11或libboost
 *
 */

#ifndef _CHEF_BASE_ENV_HPP_
#define _CHEF_BASE_ENV_HPP_
#pragma once

// #define CHEF_USE_BOOST

#ifndef CHEF_USE_BOOST
#if (__cplusplus < 201103L)
  #error("some of chef_env stuff deps on c++11 or boost, you should compile it with c++11 support or define macro CHEF_USE_BOOST if you choose to use boost instead.")
#endif
#endif

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
