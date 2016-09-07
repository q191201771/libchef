#include "../base/filepath_op.h"
#include <stdio.h>
#include <string.h>
#include <cstdlib>
#include <ctime>
#include <vector>
#include <string>
#include "./common/assert_wrapper.hpp"
#include "./common/check_log.hpp"

std::vector<std::string> gc_list;

static std::string rand_string() {
  return std::to_string(std::rand());
}

void exist_test() {
  assert(chef::filepath_op::exist("/tmp") == 0);
  std::string filename = std::string("/tmp/exist_test.") + rand_string();
  assert(chef::filepath_op::exist(filename.c_str()) == -1);
  assert(chef::filepath_op::write_file(filename.c_str(), std::string("hello\n")) == 0);
  assert(chef::filepath_op::exist(filename.c_str()) == 0);
  gc_list.push_back(filename);
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
  gc_list.push_back(pathname);
  pathname = chef::filepath_op::join("/tmp", rand_string());
  gc_list.push_back(pathname);
  pathname = chef::filepath_op::join(pathname, rand_string());
  assert(chef::filepath_op::mkdir_recursive(pathname.c_str()) == 0);
}

void rm_file_test() {
  std::string filename = std::string("/tmp");
  assert(chef::filepath_op::rm_file(filename.c_str()) == -1);
  filename += std::string("/") + rand_string();
  assert(chef::filepath_op::rm_file(filename.c_str()) == 0);
  assert(chef::filepath_op::write_file(filename.c_str(), std::string("hello\n")) == 0);
  gc_list.push_back(filename);
  assert(chef::filepath_op::rm_file(filename.c_str()) == 0);
}

void rmdir_recursive_test() {
  std::string root_path = "/tmp/rm_dir_test";
  assert(chef::filepath_op::mkdir_recursive(root_path.c_str()) == 0);

  std::string child_path = chef::filepath_op::join(root_path, rand_string());
  assert(chef::filepath_op::mkdir_recursive(child_path.c_str()) == 0);
  std::string child_path_child1 = chef::filepath_op::join(child_path, rand_string());
  assert(chef::filepath_op::mkdir_recursive(child_path_child1.c_str()) == 0);
  std::string child_path_child2 = chef::filepath_op::join(child_path, rand_string());
  assert(chef::filepath_op::write_file(child_path_child2.c_str(), std::string("hello\n")) == 0);

  std::string child_file = chef::filepath_op::join(root_path, rand_string());
  assert(chef::filepath_op::write_file(child_file.c_str(), std::string("hello\n")) == 0);

  assert(chef::filepath_op::rmdir_recursive(root_path.c_str()) == 0);
}

void rename_test() {
  std::string filename = "/tmp";
  assert(chef::filepath_op::rename(filename.c_str(), "/tmp/rename_test") == -1);
  filename += std::string("/") + rand_string();
  assert(chef::filepath_op::rename(filename.c_str(), "/tmp/rename_test") == -1);
  assert(chef::filepath_op::write_file(filename.c_str(), std::string("hello\n")) == 0);
  assert(chef::filepath_op::rename(filename.c_str(), "/tmp/rename_test") == 0);
  gc_list.push_back("/tmp/rename_test");
}

void get_file_size_test() {
  std::string filename = std::string("/tmp/") + rand_string();
  assert(chef::filepath_op::get_file_size(filename.c_str()) == -1);
  assert(chef::filepath_op::write_file(filename.c_str(), std::string("hello\n")) == 0);
  gc_list.push_back(filename);
  assert(chef::filepath_op::get_file_size(filename.c_str()) == 6);
}

void write_file_test() {
  assert(true);
}

void read_file_test() {
  const int bufsize = 128;
  char buf[bufsize] = {0};
  std::string filename = std::string("/tmp/read_file_test.") + rand_string();
  assert(chef::filepath_op::read_file(filename) == std::string());
  assert(chef::filepath_op::read_file(filename, bufsize) == std::string());
  assert(chef::filepath_op::read_file(filename.c_str(), buf, bufsize) == -1);
  assert(chef::filepath_op::write_file(filename.c_str(), std::string("hello\n")) == 0);
  gc_list.push_back(filename);
  assert(chef::filepath_op::read_file(filename.c_str(), buf, bufsize) == strlen("hello\n"));
  assert(strcmp(buf, "hello\n") == 0);
  assert(chef::filepath_op::read_file(filename) == std::string("hello\n"));
  assert(chef::filepath_op::read_file(filename, 128) == std::string("hello\n"));
}

void join_test() {
  assert(chef::filepath_op::join("a", "b") == std::string("a/b"));
  assert(chef::filepath_op::join("a", "b/c") == std::string("a/b/c"));
  assert(chef::filepath_op::join("a/b", "c") == std::string("a/b/c"));
  assert(chef::filepath_op::join("a/b", "/c") == std::string("a/b/c"));
  assert(chef::filepath_op::join("a/b/", "c") == std::string("a/b/c"));
  assert(chef::filepath_op::join("a/b/", "/c") == std::string("a/b/c"));
}

void gc() {
  for (auto &item : gc_list) {
    if (chef::filepath_op::is_dir(item.c_str()) == 0) {
      chef::filepath_op::rmdir_recursive(item.c_str());
    } else {
      chef::filepath_op::rm_file(item.c_str());
    }
  }
}

int main(){
  ENTER_TEST;

  std::srand(static_cast<unsigned int>(std::time(0)));
  join_test();
  exist_test();
  is_dir_test();
  mkdir_recursive_test();
  rm_file_test();
  rmdir_recursive_test();
  rename_test();
  get_file_size_test();
  write_file_test();
  read_file_test();
  gc();

  return 0;
}
