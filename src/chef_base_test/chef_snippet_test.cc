
#include "../chef_base/chef_snippet.hpp"
#include "./common/assert_wrapper.hpp"
#include "./common/check_log.hpp"
#include <string>

struct Foo {
  CHEF_PROPERTY(std::string, name);
  CHEF_PROPERTY(int, age);

  CHEF_PROPERTY_WITH_INIT_VALUE(int, num_of_child, 1);

  CHEF_PROPERTY_STD_LOCK(pro_lock);
  CHEF_PROPERTY_WITH_STD_LOCK(pro_lock, float, score);
  CHEF_PROPERTY_WITH_STD_LOCK(pro_lock, long, money);
};

static void example() {
  Foo f;
  f.set_name("chef");
  f.set_age(18);
  f.set_score(88.8);
  f.set_money(500 * 10000);
  std::cout << f.name() << " " << f.age() << " " << f.score() << " " << f.money() << " " << f.num_of_child() << std::endl;
}

int main() {
  ENTER_TEST;

  example();

  return 0;
}
