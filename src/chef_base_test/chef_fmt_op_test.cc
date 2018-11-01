#include "../chef_base/chef_fmt_op.hpp"
#include "./common/assert_wrapper.hpp"
#include "./common/check_log.hpp"
#include <iostream>
#include <chrono>

struct student {
  std::string name;
  int age;

  friend std::ostream &operator<<(std::ostream &os, const student &s) {
    return os << "{" << s.name << "," << s.age << "}";
  }
};

static void example() {
  std::string res = chef::fmt_op::format("I am {}, and {} year old now!", "chef", 18);
  assert(res == "I am chef, and 18 year old now!");

  student s; s.name = "yoko"; s.age = 1;
  res = chef::fmt_op::format("student info:{}", s);
  assert(res == "student info:{yoko,1}");
}

static void bench() {
  int howmany = 1000000;
  using std::chrono::high_resolution_clock;
  auto start = high_resolution_clock::now();
  for (auto i = 0; i < howmany; i++) {
    chef::fmt_op::format("I am {}, and {} year old now!", "chef", 18);
  }
  auto delta = high_resolution_clock::now() - start;
  auto delta_d = std::chrono::duration_cast<std::chrono::duration<double>>(delta).count();

  std::cout << "Elapsed: " << delta_d << "\t" << int(howmany / delta_d) << "/sec" << std::endl;
}

int main() {
  ENTER_TEST;

  example();
#if 0
  bench();
#endif

  std::string res = chef::fmt_op::format("I am {}, and {} year old now!", "chef", 18);
  assert(res == "I am chef, and 18 year old now!");

  // 实际参数少于需要的参数
  res = chef::fmt_op::format("I am {}, and {} year old now!", "chef");
  assert(res == "I am chef, and {} year old now!");

  // 没有参数
  res = chef::fmt_op::format("I am {}, and {} year old now!");
  assert(res == "I am {}, and {} year old now!");

  // 实际参数多于需要的参数
  res = chef::fmt_op::format("I am {}, and {} year old now!", "chef", 18, "fuck");
  assert(res == "I am chef, and 18 year old now!");
  return 0;
}
