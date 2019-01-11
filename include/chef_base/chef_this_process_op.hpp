/**
 * @license  this file is a part of libchef. more info see https://github.com/q191201771/libchef
 * @tag      v1.9.17
 * @file     chef_this_process_op.hpp
 * @deps     chef_filepath_op.hpp | chef_os_exec_op.hpp
 * @platform linux | macos | xxx
 *
 * @author
 *   chef <191201771@qq.com>
 *     -initial release xxxx-xx-xx
 *
 * @brief    获取当前进程维度的信息，比如进程启动时间，当前线程数，分页大小，内存占用，可执行文件路径，进程号，用户号，用户名，编译时间git版本等
 *
 */

#ifndef _CHEF_BASE_THIS_PROCESS_OP_HPP_
#define _CHEF_BASE_THIS_PROCESS_OP_HPP_
#pragma once

#include <string>
#include <stdint.h>
#include "chef_filepath_op.hpp"
#include "chef_os_exec_op.hpp"

namespace chef {

  class this_process_op {
    public:
      // 进程启动时的unix时间戳，单位秒
      static int64_t boot_timestamp();

      // 进程至今运行时间，单位秒
      static int64_t up_duration_seconds();

      // @return 成功返回进程当前线程数，失败返回-1
      static int32_t num_of_threads();

      // 内存管理的最小单位：物理页面的大小，单位字节
      static int64_t page_size();

      struct mem_info {
        int32_t virt_kbytes; // 进程当前虚拟内存大小，对应top中的VIRT列，单位KBytes
        int32_t res_kbytes;  // 进程当前常驻内存大小，对应top中的RES列，单位KBytes

        mem_info() : virt_kbytes(0), res_kbytes(0) {}
      };
      static bool obtain_mem_info(mem_info *mi);

      // func exe_filepath e.g. /home/travis/build/q191201771/libchef/output/chef_base_test/chef_this_process_op_test
      // func exe_path     e.g. /home/travis/build/q191201771/libchef/output/chef_base_test/
      // func exe_name     e.g. chef_this_process_op_test
      static std::string exe_filepath();
      static std::string exe_path();
      static std::string exe_name();

      static int32_t pid();
      static int32_t ppid();
      static int32_t uid();
      static int32_t euid();

      // 启动当前进程的用户名
      static std::string user_name();

      // 比如Makfile或者Scons脚本中拼装格式为<编译时间>_<git最后一次commit号的前7位>
      // 通过 -DCHEF_VERSION=20170322212416_516ffc1 传入
      // 如果读取不到返回"unknown"
      static std::string version();

    private:
      this_process_op();
      this_process_op(this_process_op &);
      this_process_op &operator=(this_process_op &);
  };

} // namespace chef





///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// @NOTICE 该分隔线以上部分为该模块的接口，分割线以下部分为对应的实现





#include <ctime>
#include <cstdlib>
#include <sstream>
#include <iostream>
#include <pwd.h>
#include <unistd.h>
#include <stdint.h>
#include <sys/types.h>
#ifdef __MACH__
#include <mach-o/dyld.h>
#endif

namespace chef {

namespace inner {

  static const int64_t BOOT_TIMESTAMP = std::time(NULL);

#ifdef __linux__
  class proc_stat {
    public:
      std::string string_dummy_;
      char        ch_dummy_;
      int32_t     int32_dummy_;
      int64_t     int64_dummy_;
      int32_t     virt_kbytes_;
      int32_t     res_kbytes_;

      void parse(const char *content, int64_t page_size) {
        (void)content;
        int64_t vsize = 0;
        int64_t res = 0;
        std::istringstream iss(content);
        iss >> int32_dummy_ >> string_dummy_ >> ch_dummy_ >> int32_dummy_ >> int32_dummy_
            >> int32_dummy_ >> int32_dummy_ >> int32_dummy_ >> int32_dummy_
            >> int64_dummy_ >> int64_dummy_ >> int64_dummy_ >> int64_dummy_
            >> int64_dummy_ >> int64_dummy_ >> int64_dummy_ >> int64_dummy_
            >> int64_dummy_ >> int64_dummy_ >> int64_dummy_ >> int64_dummy_ >> int64_dummy_
            >> vsize >> res >> int64_dummy_;
        virt_kbytes_ = static_cast<int32_t>(vsize / 1024);
        res_kbytes_ = static_cast<int32_t>(res * (page_size / 1024));
      }
  };

  static std::string status() {
    return filepath_op::read_file("/proc/self/status", 65535);
  }

  static std::string stat() {
    return filepath_op::read_file("/proc/self/stat", 65535);
  }
#endif // __linux__

} // namespace inner

  inline std::string this_process_op::version() {
#define MACRO_2_STRING_(x) #x
#define MACRO_2_STRING(x) MACRO_2_STRING_(x)

#ifdef CHEF_VERSION
    return std::string(MACRO_2_STRING(CHEF_VERSION));
#else
    return "unknown";
#endif
  }

  inline int32_t this_process_op::pid() {
    return static_cast<int32_t>(::getpid());
  }

  inline int32_t this_process_op::ppid() {
    return static_cast<int32_t>(::getppid());
  }

  inline int32_t this_process_op::uid() {
    return static_cast<int32_t>(::getuid());
  }

  inline int32_t this_process_op::euid() {
    return static_cast<int32_t>(::geteuid());
  }

  inline std::string this_process_op::user_name() {
    int bufsize = 256;
    char buf[256] = {0};
    struct passwd pwd;
    struct passwd *result = NULL;
    if (getpwuid_r(::getuid(), &pwd, buf, bufsize, &result) != 0 || result == NULL) {
      return std::string();
    }
    return std::string(pwd.pw_name);
  }

  inline int64_t this_process_op::boot_timestamp() {
    return inner::BOOT_TIMESTAMP;
  }

  inline int64_t this_process_op::up_duration_seconds() {
    return std::time(NULL) - inner::BOOT_TIMESTAMP;
  }

  inline int32_t this_process_op::num_of_threads() {
#ifdef __linux__
    std::string status = inner::status();
    size_t pos = status.find("Threads:");
    if (pos == std::string::npos) {
      return -1;
    }
    return std::atoi(status.c_str() + pos + 8);
#endif
#ifdef __MACH__
    int32_t pid = this_process_op::pid();
    std::ostringstream cmdss;
    cmdss << "top -l 1 -pid " << pid;
    std::vector<std::string> output_lines;
    int exit_status;
    bool res = chef::os_exec_op::run_command(cmdss.str(), &output_lines, &exit_status);
    if (!res || output_lines.empty() || exit_status != 0) { return -1; }

    std::string dummy;
    std::string numstr; // may be only `total` or `total/running`
    std::istringstream iss(output_lines.back());
    iss >> dummy >> dummy >> dummy >> dummy >> numstr;
    std::size_t pos = numstr.find("/");
    if (pos != std::string::npos) { numstr = numstr.substr(0, pos+1); }

    int32_t num = atoi(numstr.c_str());
    return num > 0 ? num : -1;
#endif
    return -1;
  }

  inline std::string this_process_op::exe_filepath() {
#ifdef __linux__
    return chef::filepath_op::read_link("/proc/self/exe", 256);
#endif
#ifdef __MACH__
    char path[1024] = {0};
    unsigned size = 1024;
    _NSGetExecutablePath(path, &size);
    return std::string(path);
#endif
    return std::string();
  }

  inline std::string this_process_op::exe_path() {
    std::string filepath = exe_filepath();
    if (filepath == std::string()) { return std::string(); }

    std::size_t pos = filepath.find_last_of('/');
    if (pos == std::string::npos) { return std::string(); }

    return filepath.substr(0, pos);
  }

  inline std::string this_process_op::exe_name() {
    std::string filepath = exe_filepath();
    if (filepath == std::string()) { return std::string(); }

    std::size_t pos = filepath.find_last_of('/');
    if (pos == std::string::npos) { return std::string(); }

    return std::string(filepath, pos + 1);
  }

  inline int64_t this_process_op::page_size() {
    return ::sysconf(_SC_PAGE_SIZE);
  }

  inline bool this_process_op::obtain_mem_info(mem_info *mi) {
    if (!mi) { return false; }

#ifdef __linux__
    inner::proc_stat ps;
    ps.parse(inner::stat().c_str(), page_size());
    mi->virt_kbytes = ps.virt_kbytes_;
    mi->res_kbytes = ps.res_kbytes_;
    return true;
#endif
#ifdef __MACH__
    int32_t pid = this_process_op::pid();
    std::ostringstream cmdss;
    cmdss << "ps -eo pid,vsz,rss | grep " << pid;
    std::vector<std::string> output_lines;
    int exit_status;
    bool res = chef::os_exec_op::run_command(cmdss.str(), &output_lines, &exit_status);
    if (!res || output_lines.size() != 1 || exit_status != 0) { return false; }

    int32_t parsed_pid = 0;
    int64_t vsz = 0;
    int64_t rss = 0;
    std::istringstream iss(output_lines.back());
    iss >> parsed_pid >> vsz >> rss;
    if (parsed_pid != pid) { return false; }

    mi->virt_kbytes = vsz;
    mi->res_kbytes = rss;
    return true;
#endif
    return false;
  }

} // namespace chef

#endif // _CHEF_BASE_THIS_PROCESS_OP_HPP_
