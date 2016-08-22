#include "../base/filepath_op.h"
#include <assert.h>
#include <stdio.h>
#include <cstdlib>
#include <ctime>

static std::string rand_string() {
  return std::to_string(std::rand());
}

void exist_test() {
  assert(chef::filepath_op::exist("/tmp") == 0);
  std::string filename = std::string("/tmp/exist_test.") + rand_string();
  assert(chef::filepath_op::exist(filename.c_str()) == -1);
  assert(chef::filepath_op::write_file(filename.c_str(), "hello\n", 6) == 0);
  assert(chef::filepath_op::exist(filename.c_str()) == 0);
}

void is_dir_test() {
  assert(chef::filepath_op::is_dir("/tmp") == 0);
  std::string pathname = std::string("/tmp/is_dir_test.") + rand_string() + std::string("/");
  assert(chef::filepath_op::is_dir(pathname.c_str()) == -1);
}

void mkdir_recursive_test() {
  std::string pathname = std::string("/tmp");
  assert(chef::filepath_op::mkdir_recursive(pathname.c_str()) == 0);
  pathname += std::string("/") + rand_string();
  assert(chef::filepath_op::mkdir_recursive(pathname.c_str()) == 0);
  pathname = std::string("/tmp/") + rand_string() + std::string("/") + rand_string();
  assert(chef::filepath_op::mkdir_recursive(pathname.c_str()) == 0);
}

void rm_file_test() {
  std::string filename = std::string("/tmp");
  assert(chef::filepath_op::rm_file(filename.c_str()) == -1);
  filename += std::string("/") + rand_string();
  assert(chef::filepath_op::rm_file(filename.c_str()) == 0);
  assert(chef::filepath_op::write_file(filename.c_str(), "hello\n", 6) == 0);
  assert(chef::filepath_op::rm_file(filename.c_str()) == 0);
}

void rm_dir_test() {
  std::string pathname = "/tmp";
  assert(chef::filepath_op::rm_dir(pathname.c_str()) == -1);
  pathname += std::string("/") + rand_string();
  assert(chef::filepath_op::rm_dir(pathname.c_str()) == 0);
  assert(chef::filepath_op::mkdir_recursive(pathname.c_str()) == 0);
  //std::string filename = pathname + std::string("/") + rand_string();
  //assert(chef::filepath_op::write_file(filename.c_str(), "hello\n", 6) == 0);
  assert(chef::filepath_op::rm_dir(pathname.c_str()) == 0);
}

void rename_test() {
  std::string filename = "/tmp";
  assert(chef::filepath_op::rename(filename.c_str(), "/tmp/rename_test") == -1);
  filename += std::string("/") + rand_string();
  assert(chef::filepath_op::rename(filename.c_str(), "/tmp/rename_test") == -1);
  assert(chef::filepath_op::write_file(filename.c_str(), "hello\n", 6) == 0);
  assert(chef::filepath_op::rename(filename.c_str(), "/tmp/rename_test") == 0);
}

void get_file_size_test() {
  std::string filename = std::string("/tmp/") + rand_string();
  assert(chef::filepath_op::get_file_size(filename.c_str()) == -1);
  assert(chef::filepath_op::write_file(filename.c_str(), "hello\n", 6) == 0);
  assert(chef::filepath_op::get_file_size(filename.c_str()) == 6);
}

void write_file_test() {
  assert(true);
}

void read_file_test() {
  std::string filename = std::string("/tmp/read_file_test.") + rand_string();
  assert(chef::filepath_op::read_file(filename) == std::string());
  assert(chef::filepath_op::write_file(filename.c_str(), "hello\n", 6) == 0);
  assert(chef::filepath_op::read_file(filename) == std::string("hello\n"));
}

int main(){
  printf("Check filepath_op.\n");
  std::srand(std::time(0));
  exist_test();
  is_dir_test();
  mkdir_recursive_test();
  rm_file_test();
  rm_dir_test();
  rename_test();
  get_file_size_test();
  write_file_test();
  read_file_test();
  printf("Check filepath_op done.\n");
  return 0;
}
