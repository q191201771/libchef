#include "../chef_base/chef_defer.hpp"
#include <map>
#include <string>
#include "./common/assert_wrapper.hpp"
#include "./common/check_log.hpp"

static void example() {
  int *foo = NULL;
  int *bar = new int(10);
  chef::defer x([&foo, &bar]() {
    if (foo != NULL) { delete foo; }
    if (bar != NULL) { delete bar; }
  });

  //if (true) { return; } // return under some condition
  if (true) { foo = new int(20); } // malloc under some condition
  if (true) { delete bar; bar = NULL; } // even free under some condition...

  // some op...
  return;
}

int main() {
  ENTER_TEST;

  example();

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

