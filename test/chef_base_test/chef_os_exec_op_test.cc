#include "chef_base/chef_os_exec_op.hpp"
#include "chef_base/chef_stringify_stl.hpp"
#include <ctime>
#include <string>
#include "./common/assert_wrapper.hpp"
#include "./common/check_log.hpp"

static void example() {
  bool succ;
  int exit_status;
  std::string filename = std::string("/tmp/os_op_test_") + std::to_string(std::time(0));

  // 使用echo命令创建一个随机名称的文件并写入两行内容
  std::string cmd_touch = std::string("echo \"hello\nworld\" >> ") + filename;
  std::vector<std::string> output_touch;
  succ = chef::os_exec_op::run_command(cmd_touch, &output_touch, &exit_status);
  assert(succ && output_touch.empty() && exit_status == 0);

  // 使用cat命令获取文件内容
  std::string cmd_cat = "cat " + filename;
  std::vector<std::string> output_cat;
  succ = chef::os_exec_op::run_command(cmd_cat, &output_cat, &exit_status);
  assert(succ && output_cat.size() == 2 && output_cat[0] == "hello" && output_cat[1] == "world" && exit_status == 0);

  // 使用rm命令删除文件
  std::string cmd_rm = "rm " + filename;
  std::vector<std::string> output_rm;
  succ = chef::os_exec_op::run_command(cmd_rm, &output_rm, &exit_status);
  assert(succ && output_rm.empty() && exit_status == 0);

  // 测试不存在的命令
  std::string cmd_not_exist = std::string("/tmp/cmd_not_exist_") + std::to_string(std::time(0));
  std::vector<std::string> output_cmd_not_exist;
  succ = chef::os_exec_op::run_command(cmd_not_exist, &output_cmd_not_exist, &exit_status);
  assert(succ && output_cmd_not_exist.empty() && exit_status != 0);

  // 测试删除不存在的文件
  std::string cmd_rm_not_exist = std::string("rm /tmp/file_not_exist_") + std::to_string(std::time(0));
  std::vector<std::string> output_cmd_rm_not_exist;
  succ = chef::os_exec_op::run_command(cmd_rm_not_exist, &output_cmd_rm_not_exist, &exit_status);
  assert(succ && output_cmd_not_exist.empty() && exit_status != 0);
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
    int exit_status;
    std::vector<std::string> output;
    bool succ = chef::os_exec_op::run_command(commands[i], &output, &exit_status);
    assert(succ && exit_status == 0);
    //printf("-----\ncommand: %s\nsucc:%d\nexit_status:%d\noutput:\n%s\n-----\n",
    //       commands[i].c_str(),
    //       succ,
    //       exit_status,
    //       chef::stringify_stl(output, chef::STRINGIFY_STL_STYLE_ONE_BEAUTY).c_str());
  }

  return 0;
}
