#include "logo.h"
#include "chef_filepath_op.h"
#include <iostream>

namespace chef {

  logo::logo(const std::string &filename)
    : filename_(filename)
    , content_(std::string())
  {}

  logo::~logo() {}

  int logo::init() {
    content_ = filepath_op::read_file(filename_);
    return content_.empty() ? -1 : 0;
  }

  void logo::flush_to_stdout() {
    std::cout << content_ << std::endl;
  }

} // namespace chef
