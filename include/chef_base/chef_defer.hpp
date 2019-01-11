/**
 * @license  this file is a part of libchef. more info see https://github.com/q191201771/libchef
 * @tag      v1.9.17
 * @file     chef_defer.hpp
 * @deps     chef_env.hpp
 * @platform linux | macos | xxx
 *
 * @author
 *   chef <191201771@qq.com>
 *     - initial release xxxx-xx-xx
 *
 * @brief    类似golang defer，支持c goto清理等场景
 *
     ```
     void example() {
       int *foo = NULL;
       int *bar = new int(10);
       chef::defer x([&foo, &bar]() {
         if (foo != NULL) { delete foo; }
         if (bar != NULL) { delete bar; }
       });

       //if (true) { return; } // return under some condition
       if (true) { foo = new int(20); } // malloc under some condition
       if (true) { delete bar; bar = NULL; } // even free under some condition...

       // some op...
       return;
     }
     ```
 *
 */

#ifndef _CHEF_BASE_DEFER_HPP_
#define _CHEF_BASE_DEFER_HPP_
#pragma once

#include "chef_env.hpp"

namespace chef {

  class defer {
    public:
      typedef chef::function<void()> task;

    public:
      explicit defer(const task &t=nullptr) : cancel_(false), t_(t) {}

      ~defer() {
        if (!cancel_ && t_) {
          t_();
        }
      }

    public:
      void cancel() { cancel_ = true; }

    private:
      defer(const defer &);
      defer &operator=(const defer &);

    private:
      bool cancel_;
      task t_;

  }; // class defer

}; // namespace chef

#endif // _CHEF_BASE_DEFER_HPP_
