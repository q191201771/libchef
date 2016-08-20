#include "json_config.h"
#include <fstream>
#include <iostream>
#include <json/reader.h>

namespace chef {

  json_config::json_config() : styled_string_(std::string()) {}
  json_config::~json_config() {}

  int json_config::load_file(const std::string &filename) {
    try {
      std::ifstream config_doc(filename, std::istream::binary);
      config_doc >> root_;
    } catch (...) {
      std::cerr << "json_config::load_file failed,filename: " << filename;
      return -1;
    }
    styled_string_ = root_.toStyledString();
    return 0;
  }

} // namespace chef
