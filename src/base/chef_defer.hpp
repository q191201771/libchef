/**
 * @tag      v1.3.4
 * @file     chef_defer.hpp
 * @deps     chef_env.hpp
 * @platform linux/macos/xxx
 *
 * @author
 *   chef <191201771@qq.com>
 *     - initial release xxxx-xx-xx
 *
 * @brief    类似golang defer，支持c goto清理等场景
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
      defer(const task &t=nullptr) : cancel_(false), t_(t) {}

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

#endif
