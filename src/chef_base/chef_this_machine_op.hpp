/**
 * @tag      v1.5.14
 * @file     chef_this_machine_op.hpp
 * @deps     chef_filepath_op.hpp
 * @platform linux | macos | xxx
 *
 * @author
 *   chef <191201771@qq.com>
 *     -initial release xxxx-xx-xx
 *
 * @brief    获取机器维度的信息，比如CPU，内存，带宽，开机时间等
 *
 */

#ifndef _CHEF_BASE_THIS_MACHINE_OP_HPP_
#define _CHEF_BASE_THIS_MACHINE_OP_HPP_
#pragma once

#include <set>
#include <string>
#include <stdint.h>
#include "chef_filepath_op.hpp"

namespace chef {

  class this_machine_op {
    public:
      // 获取cpu核数量（逻辑核而非物理核），失败返回-1
      static int32_t num_of_available_cpu_cores();

      // 获取内存信息，后缀kb表示kbytes
      struct mem_info {
        int32_t total_kb;   // total_kb   总物理内存大小
        int32_t free_kb;    // free_kb    linux和macos都对应top命令中信息
        int32_t buffers_kb; // buffers_kb linux对应top命令中信息，@TODO macos无法获取
        int32_t cached_kb;  // cached_kb  linux对应top命令中信息，@TODO macos无法获取
        int32_t used_kb;    // used_kb    linux下等于total-free-buffers-cached而不是对应top命令中信息（top中的used是把buffers和cached都加里面了），@TODO macos下也是一样，比top中的小

        mem_info() : total_kb(0), free_kb(0), buffers_kb(0), cached_kb(0), used_kb(0) {}
      };
      static bool obtain_mem_info(mem_info *mi);

      // 获取所有网卡名称
      static bool net_interfaces(std::set<std::string> *ifs);

      /**
       * 获取当前时刻网卡收发数据量，可在两个时间点调用相减得到带宽
       *
       * @param interface 网卡名称 e.g. eth0
       * @param in        传出参数，当前共收了多少字节
       * @param out       传出参数，当前共发了多少字节
       *
       * @return 成功返回true，失败返回false
       *
       */
      static bool net_interface_bytes(const std::string &interface, uint64_t *in, uint64_t *out);

      // 开机unix时间戳，单位秒，失败返回-1
      static int64_t boot_timestamp();

      // 开机至今运行时间，单位秒，失败返回-1
      static int64_t up_duration_seconds();
  };

} // namespace chef





///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// @NOTICE 该分隔线以上部分为该模块的接口，分割线以下部分为对应的实现





#ifdef __linux__
//#if 1
#include <ctime>
#include <cstdlib>
#include <cstring>
#include <sstream>
#include <unistd.h>
#include <net/if.h>
#include <netinet/in.h>
#include <sys/ioctl.h>

namespace chef {

  namespace inner {

    class netdev {
      public:
        netdev()
          : in_bytes_(0)
          , out_bytes_(0)
        {}

        uint64_t in_bytes_;
        uint64_t out_bytes_;

        bool parse(const std::string &interface) {
          std::string content = chef::filepath_op::read_file("/proc/net/dev", 65535);
          if (content.empty()) { return false; }

          std::string key = interface + ": ";
          size_t pos = content.find(key);
          if (pos == std::string::npos) { return false; }

          std::istringstream ss(std::string(content.c_str() + pos + key.length()));
          int64_t dummy;
          ss >> in_bytes_ >> dummy >> dummy >> dummy >> dummy >> dummy >> dummy >> dummy
            >> out_bytes_;

          return true;
        }
    };

    class stat {
      public:
        stat() : btime_(0) {}

        int64_t btime_;

        bool parse() {
          std::string content = chef::filepath_op::read_file("/proc/stat", 65535);
          if (content.empty()) { return false; }

          size_t pos = content.find("btime");
          if (pos == std::string::npos) { return false; }

          btime_ = std::atoi(content.c_str() + pos + strlen("btime"));
          return true;
        }
    };

    static bool __parse_mem_info_attr_from_content(const std::string &content, const std::string &k, int *v) {
      size_t pos = content.find(k);
      if (pos == std::string::npos) { return false; }

      if (v) { *v = std::atoi(content.c_str() + pos + k.length()); }

      return true;
    }

  } // namespace inner

  inline int32_t this_machine_op::num_of_available_cpu_cores() {
    return sysconf(_SC_NPROCESSORS_CONF);
  }

  inline bool this_machine_op::obtain_mem_info(mem_info *mi) {
    if (!mi) { return false; }

    std::string content = chef::filepath_op::read_file("/proc/meminfo", 65535);
    if (content.empty()) { return false; }

    if (!inner::__parse_mem_info_attr_from_content(content, "MemTotal:", &mi->total_kb)) { return false; }
    if (!inner::__parse_mem_info_attr_from_content(content, "MemFree:", &mi->free_kb)) { return false; }
    if (!inner::__parse_mem_info_attr_from_content(content, "Buffers:", &mi->buffers_kb)) { return false; }
    if (!inner::__parse_mem_info_attr_from_content(content, "Cached:", &mi->cached_kb)) { return false; }

    mi->used_kb = mi->total_kb - mi->free_kb - mi->buffers_kb - mi->cached_kb;
    return true;
  }

  inline int64_t this_machine_op::boot_timestamp() {
    inner::stat st;
    if (!st.parse()) { return -1; }

    return st.btime_;
  }

  inline int64_t this_machine_op::up_duration_seconds() {
    inner::stat st;
    if (!st.parse()) { return -1; }

    return std::time(NULL) - st.btime_;
  }

  inline bool this_machine_op::net_interface_bytes(const std::string &interface, uint64_t *in, uint64_t *out) {
    inner::netdev nd;
    if (!nd.parse(interface)) { return false; }

    if (in)  { *in = nd.in_bytes_; }
    if (out) { *out = nd.out_bytes_; }

    return true;
  }

  inline bool this_machine_op::net_interfaces(std::set<std::string> *ifs) {
    if (!ifs) { return false; }

    struct ifreq ifr[64];
    struct ifconf ifc;

    int sock = socket(AF_INET, SOCK_DGRAM, IPPROTO_IP);
    if (sock == -1) { return false; }

    ifc.ifc_buf = (char *)ifr;
    ifc.ifc_len = sizeof(ifr);
    if (ioctl(sock, SIOCGIFCONF, &ifc) == -1) { close(sock); return false; }

    struct ifreq *it = ifc.ifc_req;
    struct ifreq *end = reinterpret_cast<struct ifreq *>((char *)ifr + ifc.ifc_len);
    for (; it != end; ++it) {
      ifs->insert(it->ifr_name);
    }

    close(sock);
    return true;
  }

} // namespace chef
#endif // __linux__

#ifdef __MACH__
//#if 0
#include <iostream>
#include <unistd.h>
#include <mach/mach_host.h>
#include <mach/mach_init.h>
#include <net/if.h>
#include <net/if_types.h>
#include <net/route.h>
#include <netinet/in.h>
#include <sys/sysctl.h>
#include <sys/types.h>
#include <sys/socket.h>


namespace chef {

  inline int32_t this_machine_op::num_of_available_cpu_cores() {
    //return ::get_nprocs();
    return sysconf(_SC_NPROCESSORS_CONF);
  }

  inline bool this_machine_op::obtain_mem_info(mem_info *mi) {
    if (!mi) { return false; }

    mach_port_t ht;
    mach_msg_type_number_t count;
    int res;
    ht = mach_host_self();

    unsigned long page_size;
    res = host_page_size(ht, &page_size);
    if (res != KERN_SUCCESS) { std::cerr << "host_page_size() failed.res:" << res << std::endl; return false; }

    struct host_basic_info hostinfo;
    count = HOST_BASIC_INFO_COUNT;
    res = host_info(ht, HOST_BASIC_INFO, reinterpret_cast<host_info_t>(&hostinfo), &count);
    if (res != KERN_SUCCESS) { std::cerr << "host_info() failed.res:" << res << std::endl; return false; }

    mi->total_kb = hostinfo.max_mem / 1024;

    vm_statistics_data_t vm_info;
    count = HOST_VM_INFO_COUNT;
    res = host_statistics(ht, HOST_VM_INFO, reinterpret_cast<host_info_t>(&vm_info), &count);
    if (res != KERN_SUCCESS) { std::cerr << "host_statistics() failed.res:" << res << std::endl; return false; }

    mi->used_kb = (vm_info.active_count + vm_info.inactive_count + vm_info.wire_count) * page_size / 1024;
    mi->free_kb = (vm_info.free_count) * page_size / 1024;

    //std::cout << "active_count:" << vm_info.active_count * 4 / 1024
    //          << " inactive_count:" << vm_info.inactive_count * 4 / 1024
    //          << " wire_count:" << vm_info.wire_count * 4 / 1024
    //          << " purgeable_count:" << vm_info.purgeable_count * 4 / 1024
    //          << " free_count:" << vm_info.free_count * 4 / 1024
    //          << std::endl;

    return true;
  }

  inline int64_t this_machine_op::boot_timestamp() {
    struct timeval btime;
    size_t len = sizeof(btime);
    int mib[2] = { CTL_KERN, KERN_BOOTTIME };
    if(sysctl(mib, 2, &btime, &len, NULL, 0) < 0 ) {
        return -1;
    }
    return btime.tv_sec;
  }

  inline int64_t this_machine_op::up_duration_seconds() {
    return std::time(NULL) - boot_timestamp();
  }

  inline bool this_machine_op::net_interface_bytes(const std::string &interface, uint64_t *in, uint64_t *out) {
    if (interface.empty() || (!in && !out)) { return false; }

    unsigned int ifindex = if_nametoindex(interface.c_str());
    if (ifindex == 0) { return false; }

    int mib[] = { CTL_NET, PF_ROUTE, 0, 0, NET_RT_IFLIST2, 0 };
    size_t len;
    if (sysctl(mib, 6, NULL, &len, NULL, 0) < 0) { return false; }

    char *buf = (char *)malloc(len);
    if (sysctl(mib, 6, buf, &len, NULL, 0) < 0) { free(buf); return false; }

    char *lim = buf + len;
    char *next = NULL;
    u_int64_t ibytes = 0;
    u_int64_t obytes = 0;
    bool found = false;
    for (next = buf; next < lim; ) {
        struct if_msghdr *ifm = reinterpret_cast<struct if_msghdr *>(next);
        next += ifm->ifm_msglen;
        if (ifm->ifm_type == RTM_IFINFO2) {
            struct if_msghdr2 *if2m = (struct if_msghdr2 *)ifm;
            if (if2m->ifm_index == ifindex) {
              ibytes = if2m->ifm_data.ifi_ibytes;
              obytes = if2m->ifm_data.ifi_obytes;
              found = true;
              break;
            }
        }
    }

    if (!found) { free(buf); return false; }
    if (in) { *in = ibytes; }
    if (out) { *out = obytes; }

    free(buf);
    return true;
  }

  inline bool this_machine_op::net_interfaces(std::set<std::string> *ifs) {
    if (!ifs) { return false; }

    struct if_nameindex *ifnis = if_nameindex();
    if (!ifnis) { return false; }

    for (struct if_nameindex *p = ifnis; p->if_index != 0 && p->if_name; p += 1) {
      ifs->insert(p->if_name);
    }

    if_freenameindex(ifnis);
    return true;
  }

} // namespace chef

#endif // __MACH__


#endif // _CHEF_BASE_THIS_MACHINE_OP_HPP_
