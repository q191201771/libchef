#include "dump.h"
#include "filepath_op.h"
#include <ctime>
#include <chrono>
#include <sstream>

namespace chef {

  dump::dump()
    : filename_(std::string())
    , thread_(NULL)
    , exit_flag_(false)
  {}

  dump::~dump() {
    exit_flag_ = true;
    if (thread_) {
      thread_->join();
      thread_ = NULL;
    }
  }

  void dump::init(const std::string &filename) {
    std::vector<std::string> dummy;
    dump::init(filename, dummy);
  }

  void dump::init(const std::string &filename, const std::vector<std::string> &tags) {
    filename_ = filename;
    for (auto &tag : tags) {
      tag2num_[tag] = 0;
    }

    thread_ = std::make_shared<std::thread>(std::bind(&dump::run_in_thread, this));
  }

  void dump::add(const std::string &tag, int num) {
    std::lock_guard<std::mutex> guard(mutex_);
    auto iter = tag2num_.find(tag);
    if (iter == tag2num_.end()) {
      tag2num_[tag] = num;
    } else {
      iter->second += num;
    }
  }

  void dump::del(const std::string &tag, int num) {
    add(tag, -num);
  }

  void dump::increment(const std::string &tag) {
    add(tag, 1);
  }

  void dump::decrement(const std::string &tag) {
    del(tag, 1);
  }

  void dump::reset(const std::string &tag) {
    std::lock_guard<std::mutex> guard(mutex_);
    tag2num_[tag] = 0;
  }

  void dump::reset() {
    std::lock_guard<std::mutex> guard(mutex_);
    for (auto &iter : tag2num_) {
      iter.second = 0;
    }
  }

  void dump::run_in_thread() {
    while(!exit_flag_) {
      std::lock_guard<std::mutex> guard(mutex_);
      std::stringstream ss;

      std::time_t now = std::time(NULL);
      ss << "dump - " << std::asctime(std::localtime(&now)) << "-----\n";

      auto iter = tag2num_.begin();
      int count = 1;
      for (; iter != tag2num_.end(); iter++, count++) {
        ss << iter->first << ": " << iter->second;
        if ((count % NUM_OF_TAG_PER_LINE == 0) ||
          count == (int)tag2num_.size()
        ) {
          ss << "\n";
        } else {
          ss << " | ";
        }
      }
      ss << "\n";
      filepath_op::write_file(filename_.c_str(), ss.str());

      std::this_thread::sleep_for(std::chrono::microseconds(DUMP_INTERVAL_MS));
    }
  }

  std::map<std::string, int> dump::kv() {
    std::lock_guard<std::mutex> guard(mutex_);
    return tag2num_;
  }

} // namespace chef
