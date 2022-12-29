#include "./common/assert_wrapper.hpp"
#include "./common/check_log.hpp"
#include "chef_base/chef_debug_counter.hpp"

class Foo : public chef::debug_counter {
public:
  Foo() : CHEF_DEBUG_COUNTER_CTOR {}
  ~Foo() {}
};

class Bar : public chef::debug_counter {
public:
  Bar() : chef::debug_counter(this, __FUNCTION__) {}
  virtual ~Bar() {}
};

int main() {
  ENTER_TEST;

  Foo f1;
  Bar b1;
  Foo f2;
  Bar b2;
  Foo *f3 = new Foo();
  delete f3;
}
