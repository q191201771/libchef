/**
 * @license  this file is a part of libchef. more info see https://github.com/q191201771/libchef
 * @tag      v1.10.17
 * @file     chef_stuff_op.hpp
 * @deps     nope
 * @platform linux | macos | xxx
 *
 * @author
 *   chef <191201771@qq.com>
 *     -initial release xxxx-xx-xx
 *
 * @brief    一些暂时没归类的功能代码片段
 *
 */

#ifndef _CHEF_BASE_STUFF_OP_HPP_
#define _CHEF_BASE_STUFF_OP_HPP_
#pragma once

#include <string>

namespace chef {

  class stuff_op {
    public:
      /**
       * 将以字节为单位的数字转换为带大小单位的可读性字符串
       *
       * @param n bytes
       *
       * @return
       *   e.g.
       *   readable_bytes(768) = "768.0B"
       *   readable_bytes(10000) = "9.8K"
       *   readable_bytes(100001221) = "95.4M"
       *
       */
      static std::string readable_bytes(uint64_t n);

      // 将字节流逐个序列化为16进制FF格式，空格分开，<num_per_line>换行，可用于debug显示
      static std::string bytes_to_hex(const uint8_t *buf, std::size_t len, std::size_t num_per_line=8, bool with_ascii=true);

      /**
       * 获取域名对应的ip
       *
       * @param domain 域名
       *
       * @return ip
       *   成功例子：
       *     "www.baidu.com"
       *     "localhost"
       *     "58.96.168.38"
       *   失败例子：
       *     "not exist"
       *     "http://www.baidu.com"
       *
       */
      static std::string get_host_by_name(const char *domain);

      // 获取当前时间点，单位毫秒，一般用于计算两个时间点间的间隔用
      static uint64_t tick_msec();

      static uint64_t unix_timestamp_msec();

      // 获取线程号
      static int gettid();

      /**
       * 设置线程名（对应top命令Threads中的COMMAND列）
       * @NOTICE 只在linux平台生效
       *
       */
      static void set_thread_name(const char *name);

      //static int read_be_int(const uint8_t *data, uint32_t n);

    private:
      stuff_op();
      stuff_op(const stuff_op &);
      stuff_op &operator=(const stuff_op &);
  };

} // namespace chef





///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// @NOTICE 该分隔线以上部分为该模块的接口，分割线以下部分为对应的实现





#include <assert.h>
#include <netdb.h>
#include <pthread.h>
#include <time.h>
#include <unistd.h>
#include <sys/syscall.h>
#include <sys/time.h>
#include <sys/types.h>
#include <string>
#include <sstream>
#include <iomanip>
#if defined(__linux__)
#include <sys/prctl.h>
#endif

namespace chef {

inline int stuff_op::gettid() {
  static __thread int tid = 0;
  if (tid != 0) { return tid; }

#if defined(__linux__)
  return static_cast<int>(syscall(SYS_gettid));
#elif defined(__MACH__)
  uint64_t tid64;
  pthread_threadid_np(NULL, &tid64);
  return static_cast<int>(tid64);
#else
  return 0;
#endif
}

inline void stuff_op::set_thread_name(const char *name) {
#if defined(__linux__)
  ::prctl(PR_SET_NAME, name);
#else
  (void)name;
#endif
}

inline std::string stuff_op::readable_bytes(uint64_t n) {
  char UNITS[] = {'B', 'K', 'M', 'G', 'T', 'P', 'E'}; // 'Z' 'Y'
  int index = 0;
  for (; n >> (index*10); index++) {
    if (index == 6) { /// can't do n >> 70
      ++index;
      break;
    }
  }
  index = index > 0 ? index - 1 : index;
  char buf[128] = {0};
  snprintf(buf, 127, "%.1f%c",
    static_cast<float>(n) / static_cast<float>(index ? (1UL << (index*10)) : 1),
    UNITS[index]
  );
  return std::string(buf);
}

inline std::string stuff_op::get_host_by_name(const char *domain) {
  struct hostent *ht = ::gethostbyname(domain);
  if (ht == NULL || ht->h_length <= 0) { return std::string(); }

  char result[64] = {0};
  snprintf(result, 63, "%hhu.%hhu.%hhu.%hhu",
    static_cast<uint8_t>(ht->h_addr_list[0][0]),
    static_cast<uint8_t>(ht->h_addr_list[0][1]),
    static_cast<uint8_t>(ht->h_addr_list[0][2]),
    static_cast<uint8_t>(ht->h_addr_list[0][3])
  );
  return std::string(result);
}

inline uint64_t stuff_op::tick_msec() {
  struct timespec ts;
#if defined(CLOCK_REALTIME) && !defined(__MACH__)
#pragma message("Has defined CLOCK_REALTIME and not __MACH__.")
  clock_gettime(CLOCK_MONOTONIC, &ts);
#else
  {
    struct timeval tv;
    gettimeofday(&tv, NULL);
    ts.tv_sec = tv.tv_sec;
    ts.tv_nsec = tv.tv_usec * 1000;
  }
#endif
  return ts.tv_sec * 1000 + ts.tv_nsec / 1000000;
}

inline uint64_t stuff_op::unix_timestamp_msec() {
  struct timeval tv;
  gettimeofday(&tv, NULL);
  return tv.tv_sec * 1000 + tv.tv_usec / 1000;
}

inline std::string stuff_op::bytes_to_hex(const uint8_t *buf, std::size_t len, std::size_t num_per_line, bool with_ascii) {
  if (!buf || len == 0 || num_per_line == 0) { return std::string(); }

  static const std::string PRINTABLE_ASCII = "0123456789abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ!\"#$%&\'()*+,-./:;<=>?@[\\]^_`{|}~ ";

  std::size_t num_of_line = len / num_per_line;
  if (len % num_per_line) { num_of_line++; }

  std::ostringstream oss;
  for (std::size_t i = 0; i < num_of_line; i++) {
    std::size_t item = (len % num_per_line) && (i == num_of_line-1) ? (len % num_per_line) : num_per_line;
    std::ostringstream line_oss;
    for (std::size_t j = 0; j < item; j++) {
      if (j != 0) { line_oss << ' '; }

      line_oss << std::setfill('0') << std::setw(2) << std::hex << static_cast<int>(buf[i * num_per_line + j]);
    }

    if ( (len % num_per_line) && (i == num_of_line-1) && with_ascii ) {
      oss << std::left << std::setw(num_per_line * 2 + num_per_line - 1) << line_oss.str();
    } else {
      oss << line_oss.str();
    }

    if (with_ascii) {
      oss << "    ";
      for (std::size_t j = 0; j < item; j++) {
        if (PRINTABLE_ASCII.find(buf[i*num_per_line + j]) !=std::string::npos) {
          oss << buf[i*num_per_line + j];
        } else {
          oss << '.';
        }
      }
    }

    if (i != num_of_line) { oss << '\n'; }
  }

  return oss.str();
}

//inline int stuff_op::read_be_int(const uint8_t *data, uint32_t n) {
//  assert(n <= 4);
//  int val(0);
//  for (unsigned int i = 0; i < n; ++i) { val |= static_cast<int>(data[i]) << ((n-1-i) * 8); }
//
//  return val;
//}

}

#endif
