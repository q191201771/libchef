/**
 * @file   chef_env.hpp
 * @deps   boost or c++11
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

// #define CHEF_USE_BOOST

#ifdef CHEF_USE_BOOST

  #include <boost/thread.hpp>
    namespace chef {
      using boost::thread;
      using boost::mutex;
      using boost::lock_guard;
      using boost::unique_lock;
      using boost::condition_variable;
      namespace this_thread = boost::this_thread;
    } /// namespace chef

  #include <boost/atomic.hpp>
    namespace chef {
      using boost::atomic;
    }

  #include <boost/smart_ptr.hpp>
    namespace chef {
      using boost::shared_ptr;
      using boost::make_shared;
    }

  #include <boost/functional.hpp>
    namespace chef {
      using boost::bind;
      using boost::function;
    }

  #include <boost/chrono.hpp>
    namespace chef {
      namespace chrono = boost::chrono;
    }

#else /// > c++11

  #include <mutex>
  #include <thread>
  #include <condition_variable>
    namespace chef {
      using std::thread;
      using std::mutex;
      using std::lock_guard;
      using std::unique_lock;
      using std::condition_variable;
      namespace this_thread = std::this_thread;
    } /// namespace chef

  #include <atomic>
    namespace chef {
      using std::atomic;
    }

  #include <memory>
    namespace chef {
      using std::shared_ptr;
      using std::make_shared;
    }

  #include <functional>
    namespace chef {
      using std::bind;
      using std::function;
    }

  #include <chrono>
    namespace chef {
      namespace chrono = std::chrono;
    }

#endif /// ifdef CHEF_USE_BOOST ... else ... CHEF_USE_BOOST

#endif /// _CHEF_BASE_ENV_HPP_
