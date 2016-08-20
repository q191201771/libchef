#ifndef _CHEF_BASE_DAEMON_H_
#define _CHEF_BASE_DAEMON_H_

namespace chef {

  class daemon {
    public:
      /**
       * @return  成功返回当前进程id，失败则直接退出进程(进程已存在或调用系统函数失败)
       */
      static int start(const char *pidfile);
  };

} // namespace chef

#endif // _CHEF_BASE_DAEMON_H_
