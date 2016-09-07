#include "../base/noncopyable.hpp"
#include <stdio.h>
#include "./common/check_log.hpp"

class A : public chef::noncopyable {};

class B : public chef::copyable {};

int main() {
  ENTER_TEST;

  A a1;
  A a2;
  //a2 = a1;
  //A a3(a1);
  B b1;
  B b2;
  b2 = b1;
  B b3(b1);

  return 0;
}
