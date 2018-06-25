#include "../base/chef_log.h"
#include "./common/check_log.hpp"

int main() {
  ENTER_TEST;

  chef::log::init(chef::log::mode_debug);
  CHEF_LOG(info) << "log_test.";

  return 0;
}
