/**
 * @file     chef_log.hpp[_impl]
 * @deps     libboost | chef_env.hpp
 * @platform linux/macos/xxx
 *
 * @author
 *   chef <191201771@qq.com>
 *     -initial release xxxx-xx-xx
 *
 * @brief
 *   近乎零配置的日志模块，有其他需求请直接修改源码。
 *
 *
 *   init时mode_debug和mode_release的区别
 *     一般线上跑mode_release，开发环境跑mode_debug。
 *     1. mode_debug会打印所有6个级别的日志，mode_release只打印包含info以上的4个级别日志。
 *     2. 内容格式稍有区别，mode_debug会打印日志所在源码文件名和行号，mode_release不会。
 *     3. mode_debug的每行日志都会同步刷到文件中，mode_release是每秒刷一次。
 *
 *
 *   日志文件大小超过1GB或者每天晚上12点，会翻滚生成一个新的日志文件。
 *   是线程安全的。
 *
 */

#ifndef _CHEF_BASE_LOG_H_
#define _CHEF_BASE_LOG_H_
#pragma once

#include <boost/log/common.hpp>
#include <boost/log/core.hpp>
#include <boost/log/sinks.hpp>
#include <boost/log/attributes.hpp>
#include <boost/log/expressions.hpp>
#include <boost/log/trivial.hpp>
#include <boost/log/utility/setup/common_attributes.hpp>
#include <boost/log/utility/exception_handler.hpp>
#include <boost/log/support/date_time.hpp>

namespace chef {

  class log {
    public:
      enum mode {
        mode_debug,
        mode_release,
      };

      /**
       * @param filename_with_path
       *   如果设置，则为日志输出文件名
       *     支持目录，如果目录不存在，将创建
       *     文件名支持格式化日期及翻滚编号，例如 "test_chef_log.log.%Y%m%dT%H%M%S.%N"
       *   如果不设置，日志文件名由 执行文件名+日志文件生成时间+翻滚编号+机器名+进程号+扩展后缀组成，例如
       *     test_chef_log.20150415T160553.0.chef-VirtualBox.27945.log.chef
       *
       */
      static void init(mode m, const std::string &filename_with_path=std::string());

      // 当运行在release mode时，日志为异步刷盘，可以调用这个函数强制刷盘，比如说程序异常退出时，尽量避免日志丢失
      static void force_flush();

    public:
      static boost::log::trivial::severity_level &get_level();

  };

  namespace internal {
    boost::log::sources::severity_logger_mt<boost::log::trivial::severity_level> &get_logger();
  } // namespace internal

} // namespace chef

#define CHEF_LOG(level) \
  BOOST_LOG_FUNCTION(); \
  if (boost::log::trivial::level >= chef::log::get_level()) BOOST_LOG_SEV(chef::internal::get_logger(), boost::log::trivial::level)

#include "chef_log_impl.hpp"

#endif // _CHEF_BASE_LOG_H_
