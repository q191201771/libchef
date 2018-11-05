#include "../chef_base/chef_daemon_op.hpp"
#include "./common/assert_wrapper.hpp"
#include "./common/check_log.hpp"

int main() {
  {
  ENTER_TEST;
  }

  pid_t pid = ::getpid();
  assert(pid > 0);
  //printf("pid=%d\n", pid);
  int pid2 = chef::daemon_op::start("/tmp/daemon_test.pid");
  assert(pid2 > 0 && pid != pid2);
  /// 我们在守护进程里了~
  sleep(5);
  return 0;
}
