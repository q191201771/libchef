// Copyright 2022, Yoko.  All rights reserved.
//
// Author: Yoko (191201771@qq.com)

// 解析url字符串
//
// 示例如下（更多见单元测试）：
//
//  chef::url_context ctx;
//  int res = chef::url_context::parse("http://127.0.0.1:80/live/test110.flv?a=1&b=2", ctx);
//  chef::url_context expect;
//  assert(res == 0);
//  assert(ctx.url_ == "http://127.0.0.1:80/live/test110.flv?a=1&b=2");
//  assert(ctx.scheme_ == "http");
//  assert(ctx.stdhost_ == "127.0.0.1:80");
//  assert(ctx.host_ == "127.0.0.1");
//  assert(ctx.origin_port_ == 80);
//  assert(ctx.path_with_raw_query_ == "/live/test110.flv?a=1&b=2");
//  assert(ctx.path_ == "/live/test110.flv");
//  assert(ctx.path_without_last_item_ == "live");
//  assert(ctx.last_item_of_path_ == "test110.flv");
//  assert(ctx.raw_query_ == "a=1&b=2");

#ifndef CHEF_BASE_URL_HPP_
#define CHEF_BASE_URL_HPP_
#pragma once

#include <string>
#include "chef_strings_op.hpp"

namespace chef {

  class url_context {
  public:
    static int parse(const std::string &url, url_context &ctx);

  public:
    std::string url_;
    std::string scheme_;
    std::string stdhost_;     // 原始url中包含port就是host:port，没有port则是host
    std::string host_;        // 不包含port
    int         origin_port_; // 原始url中包含port就是port的值，没有port则为0
    std::string path_with_raw_query_;     // 注意，有前面的'/'
    std::string path_;                    // 注意，有前面的'/'
    std::string path_without_last_item_;  // 注意，没有前面的'/'，也没有后面的'/'
    std::string last_item_of_path_;       // 注意，没有前面的'/'
    std::string raw_query_;               // 参数，注意，没有前面的'?'

//    std::string username_;
//    std::string password_;
  };

  inline int url_context::parse(const std::string &url, url_context &ctx) {
    ctx.url_ = url;

    std::string str;
    chef::strings_op::cut(url, "://", &ctx.scheme_, &str);

    chef::strings_op::cut(str, "/", &ctx.stdhost_, &str);

    std::string port;
    chef::strings_op::cut(ctx.stdhost_, ":", &ctx.host_, &port);
    if (!port.empty()) {
      ctx.origin_port_ = atoi(port.c_str());
    }

    ctx.path_with_raw_query_ = "/" + str;

    chef::strings_op::cut(ctx.path_with_raw_query_, "?", &ctx.path_, &ctx.raw_query_);
    chef::strings_op::cut_last(ctx.path_, "/", &ctx.path_without_last_item_, &ctx.last_item_of_path_);
    ctx.path_without_last_item_ = chef::strings_op::trim_prefix(ctx.path_without_last_item_, "/");
    return 0;
  }

}

#endif