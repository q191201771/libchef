#include "../base/noncopyable.hpp"
#include <stdio.h>

class A : public chef::noncopyable {};

class B : public chef::copyable {};

int main() {
  printf("Check noncopyable.\n");
  
  A a1;
  A a2;
  //a2 = a1;
  //A a3(a1);
  B b1;
  B b2;
  b2 = b1;
  B b3(b1);

  printf("Check noncopyable done.\n");
  return 0;
}
