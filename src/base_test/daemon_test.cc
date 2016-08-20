#include "../base/daemon.h"
#include <unistd.h>

int main() {
  chef::daemon::start("/tmp/daemon_test.pid");

  sleep(60 * 60 * 24);
  return 0;
}
