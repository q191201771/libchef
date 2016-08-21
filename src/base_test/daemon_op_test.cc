#include "../base/daemon_op.h"
#include <unistd.h>

int main() {
  chef::daemon_op::start("/tmp/daemon_test.pid");

  sleep(60 * 60 * 24);
  return 0;
}
