/**
 * @author chef <191201771@qq.com>
 * @deps   nope
 */
 
#ifndef _CHEF_BASE_THIS_THREAD_OP_HPP_
#define _CHEF_BASE_THIS_THREAD_OP_HPP_

namespace chef {

  /**
   * @class this_this_op
   *
   * std::this_thread由于要满足跨平台通用性,牺牲了一些linux专有特征,这里做个补充
   */
  class this_thread_op {
    public:
      /**
       * 可对应top命令Threads中的PID列
       */
      static int gettid();

      /**
       * 可对应top命令Threads中的COMMAND列
       */
      static void set_thread_name(const char *name);
  };

} // namespace chef

#endif // _CHEF_BASE_THIS_THREAD_OP_HPP_
