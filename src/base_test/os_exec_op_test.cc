#include "../base/os_exec_op.h"

#include <ctime>
#include <string>
#include <assert.h>

std::string printableResult(const std::vector<std::string> vec) {
  std::string ret;
  int num = vec.size();
  for (int i = 0; i < num; i++) {
    ret += "(";
    ret += vec[i];
    ret += ")\n";
  }
  return ret;
}

int main() {
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

  printf("Check os_exec_op done.\n");
  return 0;

  std::vector<std::string> commands = {
    std::string("ps -ef"),
    std::string("ls /tmp"),
    std::string("ls -lrt")
  };

  int command_num = commands.size();
  for (int i = 0; i < command_num; i++) {
    std::vector<std::string> result = chef::os_exec_op::run_command(commands[i]);
    printf("-----\ncommand: %s\noutput:\n%s-----\n", commands[i].c_str(), printableResult(result).c_str());
  }

  return 0;
}
