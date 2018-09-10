#include "../base/chef_os_exec_op.hpp"
#include "../base/chef_stringify_stl.hpp"
#include <ctime>
#include <string>
#include "./common/assert_wrapper.hpp"
#include "./common/check_log.hpp"

static void example() {
  std::time_t time = std::time(0);
  std::string filename = "/tmp/os_op_test." + std::to_string(time);
  std::string touch = "echo \"hello\nworld\" >> " + filename;
  std::vector<std::string> touch_result = chef::os_exec_op::run_command(touch);
  assert(touch_result.size() == 0);
  std::string cat = "cat " + filename;
  std::vector<std::string> cat_result = chef::os_exec_op::run_command(cat);
  assert(cat_result.size() == 2);
  assert(cat_result[0] == "hello");
  assert(cat_result[1] == "world");
  std::string rm = "rm " + filename;
  chef::os_exec_op::run_command(rm);
}

int main() {
  ENTER_TEST;

  example();

  std::vector<std::string> commands = {
    std::string("ps -ef | grep test"),
    std::string("ls /tmp")
  };

  uint64_t command_num = commands.size();
  for (uint64_t i = 0; i < command_num; i++) {
    std::vector<std::string> result = chef::os_exec_op::run_command(commands[i]);
    //printf("-----\ncommand: %s\noutput:\n%s\n-----\n",
    //       commands[i].c_str(),
    //       chef::stringify_stl_vector(result, "[\n",    "  \"",   "\"",    ",\n",    "\n]").c_str());
  }

  return 0;
}
