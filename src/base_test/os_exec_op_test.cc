#include "../base/os_exec_op.h"

/// TODO touch an file,cat and assert content.

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
