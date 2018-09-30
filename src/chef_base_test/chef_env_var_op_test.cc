#include "../chef_base/chef_env_var_op.hpp"
#include "./common/assert_wrapper.hpp"
#include "./common/check_log.hpp"
#include <string>
#include <ctime>

static void example() {
     bool succ;
     std::string v;

     // 获取用户根目录
     succ = chef::env_var_op::getenv("HOME", &v);
     assert(succ);
     //std::cout << succ << "," << v << std::endl;
     // 获取不存在的环境变量
     std::string k = std::string("chef_env_var_op_test_k_") + std::to_string(std::time(0));
     succ = chef::env_var_op::getenv(k, &v);
     assert(!succ);
     // 删除不存在的环境变量
     succ = chef::env_var_op::unsetenv(k);
     assert(succ);
     // 设置不存在的环境变量
     succ = chef::env_var_op::setenv(k, "111");
     assert(succ);
     // 获取存在的环境变量
     succ = chef::env_var_op::getenv(k, &v);
     assert(succ && v == "111");
     // 删除存在的环境变量
     succ = chef::env_var_op::unsetenv(k);
     assert(succ);
     // 获取已删除的环境变量
     succ = chef::env_var_op::getenv(k, &v);
     assert(!succ);
}

static void ut() {}

int main() {
  ENTER_TEST;

  example();

  ut();

  return 0;
}

