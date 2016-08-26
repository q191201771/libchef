/**
 * @file   daemon.h/daemon.cc
 * @deps   nope
 *
 * @author
 *   chef <191201771@qq.com>
 *     -initial release xxxx-xx-xx
 *
 * @brief  nope
 *
 */

#ifndef _CHEF_BASE_DAEMON_OP_H_
#define _CHEF_BASE_DAEMON_OP_H_

namespace chef {

  class daemon_op {
    public:
      /**
       * @return  成功返回当前进程id，失败则直接退出进程(进程已存在或调用系统函数失败)
       *
       */
      static int start(const char *pidfile);

    private:
      /**
       * 判断pidfile中pid对应的进程是否存在
       *
       * @return 存在返回pidfile中的pid，不存在返回-1
       *
       */
      static int exist(const char *pidfile);

      /**
       * 将当前pid写入文件
       *
       * @return 成功返回进程id，失败返回-1
       *
       */
      static int write(const char *pidfile);
  };

} // namespace chef

#endif // _CHEF_BASE_DAEMON_OP_H_
