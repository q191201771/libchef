/**
 * @file   log.h/log.cc
 * @deps   boost
 *
 * @author
 *   chef <191201771@qq.com>
 *     -initial release xxxx-xx-xx
 *
 * @brief
 *   近乎零配置的日志模块，有其他需求请直接修改源码。
 *
 *   一共两个接口，使用方法
     chef::log::init(chef::log::mode_debug);
     CHEF_LOG(trace)   << "engine by boost::log,i'm trace-severity";
     CHEF_LOG(debug)   << "engine by boost::log,i'm debug-severity";
     CHEF_LOG(info)    << "engine by boost::log,i'm info-severity";
     CHEF_LOG(warning) << "engine by boost::log,i'm warning-severity";
     CHEF_LOG(error)   << "engine by boost::log,i'm error-severity";
     CHEF_LOG(fatal)   << "engine by boost::log,i'm fatal-severity";
 *
 *   init时mode_debug和mode_release的区别
 *     一般线上跑mode_release，开发环境跑mode_debug。
 *     mode_debug会打印所有6个级别的日志，mode_release只打印包含info以上的4个级别日志。
 *     内容格式稍有区别，mode_debug会打印日志所在源码文件名和行号，mode_release不会。
 *
 *   release_mode举例
 *     时间 线程号 级别 | 内容
2016-08-20 19:09:51.030432 18712    info | engine by boost::log,i'm info-severity
2016-08-20 19:09:51.030529 18712 warning | engine by boost::log,i'm warning-severity
2016-08-20 19:09:51.030546 18712   error | engine by boost::log,i'm error-severity
2016-08-20 19:09:51.030555 18712   fatal | engine by boost::log,i'm fatal-severity
 *   debug_mode举例
 *     [时间] [线程] [级别] [源码文件:源码行号] | 内容
[2016-08-20 19:12:35.428516] [18868] [trace] [src/base/log.cc:76] | engine by boost::log,i'm trace-severity
[2016-08-20 19:12:35.428634] [18868] [debug] [...->src/base/log.cc:77] | engine by boost::log,i'm debug-severity
[2016-08-20 19:12:35.428646] [18868] [info] [...->src/base/log.cc:78] | engine by boost::log,i'm info-severity
[2016-08-20 19:12:35.428652] [18868] [warning] [...->src/base/log.cc:79] | engine by boost::log,i'm warning-severity
[2016-08-20 19:12:35.428657] [18868] [error] [...->src/base/log.cc:80] | engine by boost::log,i'm error-severity
[2016-08-20 19:12:35.428662] [18868] [fatal] [...->src/base/log.cc:81] | engine by boost::log,i'm fatal-severity
 *
 *   日志文件名由 执行文件名+日志文件生成时间+翻滚编号+机器名+进程号+扩展后缀组成，例如
 *   test_chef_log.20150415T160553.0.chef-VirtualBox.27945.log.chef
 *
 *   日志文件大小超过1GB或者每天晚上12点，会自动生成一个新的日志文件。
 *   是线程安全的。
 *   不支持跨平台。
 */

#ifndef _CHEF_BASE_LOG_H_
#define _CHEF_BASE_LOG_H_

#include "boost/log/common.hpp"
#include "boost/log/core.hpp"
#include "boost/log/sinks.hpp"
#include "boost/log/attributes.hpp"
#include "boost/log/expressions.hpp"
#include "boost/log/trivial.hpp"
#include "boost/log/utility/setup/common_attributes.hpp"
#include "boost/log/utility/exception_handler.hpp"
#include "boost/log/support/date_time.hpp"

namespace chef {

  class log {
    public:
      enum mode {
        mode_debug,
        mode_release,
      };

      static void init(mode m);
  };

  namespace internal {
    boost::log::sources::severity_logger_mt<boost::log::trivial::severity_level> &get_logger();
  } // namespace internal

} // namespace chef

#define CHEF_LOG(level) BOOST_LOG_FUNCTION();BOOST_LOG_SEV(chef::internal::get_logger(), boost::log::trivial::level)

#endif // _CHEF_BASE_LOG_H_
