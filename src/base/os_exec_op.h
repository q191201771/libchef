/**
 * @file   os_exec_op.h/os_exec_op.cc
 * @deps   nope
 *
 * @author
 *   chef <191201771@qq.com>
 *     -initial release 2016-08-31
 *
 * @brief  nope
 *
 */

#ifndef _CHEF_BASE_OS_EXEC_OP_H_
#define _CHEF_BASE_OS_EXEC_OP_H_

#include <string>
#include <vector>

namespace chef {

  class os_exec_op {
    public:
      /**
       * 执行shell命令，返回包含每行执行结果的数组
       *
       */
      static std::vector<std::string> run_command(const std::string &cmd);
  };

} // namespace chef

#endif // _CHEF_BASE_OS_EXEC_OP_H_
