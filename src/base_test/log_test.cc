#include "../base/log.h"

int main() {
  printf("Check log.\n");
  chef::log::init(chef::log::mode_debug);
  CHEF_LOG(info) << "log_test.";

  printf("Check log done.\n");
  return 0;
}
