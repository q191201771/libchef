/**
 * @file   this_machine_op.h/this_machine_op.cc
 * @deps   nope
 *
 * @author
 *   chef <191201771@qq.com>
 *     -initial release xxxx-xx-xx
 *
 * @brief
 *   获取当前机器的相关信息
 *
 */

#ifndef _CHEF_BASE_THIS_MACHINE_OP_H_
#define _CHEF_BASE_THIS_MACHINE_OP_H_

#include <string>
#include <stdint.h>

namespace chef {

  class this_machine_op {
    public:
      static int32_t num_of_available_cpu_cores();

      /**
       * @function mem_total_kb，mem_free_kb，mem_buffers_kb，mem_cached_kb，mem_used_kb
       *
       * 内存信息
       *
       * used = total - free - buffers - cached
       *
       * @return 失败返回0
       *
       */
      static int32_t mem_total_kb();
      static int32_t mem_free_kb();
      static int32_t mem_buffers_kb();
      static int32_t mem_cached_kb();
      static int32_t mem_used_kb();

      /**
       * 获取当前时刻网卡收发数据量，可在两个时间点调用相减得到带宽
       *
       * @param interface 网卡名称 e.g. eth0
       * @param in        传出参数，当前共收了多少字节
       * @param out       传出参数，当前共发了多少字节
       *
       * @return 成功返回0，失败返回-1
       *
       */
      static int io_bytes(const std::string &interface, uint64_t *in, uint64_t *out);

      /**
       * 开机unix时间戳
       *
       * @return 失败返回0
       *
       */
      static int64_t boot_timestamp();

      /**
       * 开机至今运行时间，单位秒
       *
       * @return 失败返回0
       */
      static int64_t up_duration_seconds();
  };

} // namespace chef

#endif // _CHEF_BASE_THIS_MACHINE_OP_H_
