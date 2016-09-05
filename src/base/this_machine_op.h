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

namespace chef {

  class this_machine_op {
    public:
      static int num_of_available_cpu_cores();

      /**
       * used = total - free - buffers - cached
       *
       */
      static int mem_total_kb();
      static int mem_free_kb();
      static int mem_buffers_kb();
      static int mem_cached_kb();
      static int mem_used_kb();

      static int boot_timestamp();

      static int up_duration_seconds();
  };

} // namespace chef

#endif // _CHEF_BASE_THIS_MACHINE_OP_H_
