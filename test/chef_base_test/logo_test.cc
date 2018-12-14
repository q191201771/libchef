#include "chef_base/logo.h"
#include "./common/assert_wrapper.hpp"
#include "./common/check_log.hpp"

int main() {
  ENTER_TEST;

  FLUSH_CHEF_LOGO;
  //chef::logo_op("chef.logo").init().flush_to_stdout();

  return 0;
}
