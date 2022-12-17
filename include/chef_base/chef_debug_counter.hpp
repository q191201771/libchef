// Copyright 2022, Yoko.  All rights reserved.
//
// Author: Yoko (191201771@qq.com)

// 用于调试对象是否正常释放的计时器

#ifndef CHEF_BASE_DEBUG_COUNTER_HPP_
#define CHEF_BASE_DEBUG_COUNTER_HPP_
#pragma once

#include <sstream>
#include <string>

namespace chef {

  namespace inner {
    template<class Dummy>
    struct debug_counter_static {
      static std::atomic<std::uint64_t> total_;
      static std::function<void(std::string)> cb_;
    };

    template<class Dummy>
    std::atomic<std::uint64_t> debug_counter_static<Dummy>::total_;

    template<class Dummy>
    std::function<void(std::string)> debug_counter_static<Dummy>::cb_;
  }

  class debug_counter : private inner::debug_counter_static<void> {
  public:
    debug_counter(void *ptr, const std::string &tag);
    virtual ~debug_counter();

    static void with_callback(std::function<void(std::string)> cb);

  private:
    std::string info_;
  };

#define CHEF_DEBUG_COUNTER_CTOR() chef::debug_counter(this, __FUNCTION__)

// ---------------------------------------------------------------------------------------------------------------------

inline debug_counter::debug_counter(void *ptr, const std::string &tag) {
  total_++;

  std::ostringstream oss;
  oss << "[" << ptr << "], [" << tag << "]";
  info_ = oss.str();

  oss.str("");
  oss << "DEBUG_COUNTER CTOR " << total_ << ", " << info_;
  if (cb_) {
    cb_(oss.str());
  } else {
    fprintf(stderr, "%s\n", oss.str().c_str());
  }
}

inline debug_counter::~debug_counter() {
  total_--;

  std::ostringstream oss;
  oss << "DEBUG_COUNTER DTOR " << total_ << ", " << info_;
  if (cb_) {
    cb_(oss.str());
  } else {
    fprintf(stderr, "%s\n", oss.str().c_str());
  }
}

inline void debug_counter::with_callback(std::function<void(std::string)> cb) {
  cb_ = cb;
}

}

#endif
