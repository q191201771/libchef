#include "../base/chef_defer.hpp"
#include <map>
#include <string>
#include "./common/assert_wrapper.hpp"
#include "./common/check_log.hpp"

int main() {
  ENTER_TEST;

  int a = 10;
  std::string str = "aaa";
  std::string f = __FUNCTION__;
  chef::defer x([&a, &str, &f]() {
    //printf("here. %d %s %s\n", a, str.c_str(), f.c_str());
    //str = "ccc";
  });

  str = "bbb";
  //printf("%d %s\n", a, str.c_str());

  if (1) {
    return 0;
  }

  if (0) {
    x.cancel();
    return 0;
  }

  return 0;
}

