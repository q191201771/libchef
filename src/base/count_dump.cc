#include "count_dump.h"
#include "filepath_op.h"
#include <ctime>
#include <sstream>

namespace chef {

  count_dump::count_dump()
    : filename_(std::string())
    , dump_interval_ms_(-1)
    , exit_flag_(false)
  {}

  count_dump::~count_dump() {
    exit_flag_ = true;
    if (thread_) {
      thread_->join();
      thread_.reset();
    }
  }

  void count_dump::init(const std::string &filename, int32_t dump_interval_ms) {
    std::vector<std::string> dummy;
    count_dump::init(filename, dummy, dump_interval_ms);
  }

  void count_dump::init(const std::string &filename, const std::vector<std::string> &tags, int32_t dump_interval_ms) {
    dump_interval_ms_ = dump_interval_ms;
    filename_ = filename;
//    for (auto &tag : tags) {
//      tag2num_[tag] = 0;
//    }
    std::vector<std::string>::const_iterator iter = tags.begin();
    for (; iter != tags.end(); iter++) {
      tag2num_[*iter] = 0;
    }

    thread_ = chef::make_shared<chef::thread>(chef::bind(&count_dump::run_in_thread_, this));
  }

  void count_dump::add(const std::string &tag, int num) {
    chef::lock_guard<chef::mutex> guard(mutex_);
//    auto iter = tag2num_.find(tag);
    tag2num_iterator iter = tag2num_.find(tag);
    if (iter == tag2num_.end()) {
      tag2num_[tag] = num;
    } else {
      iter->second += num;
    }
  }

  void count_dump::del(const std::string &tag, int num) {
    add(tag, -num);
  }

  void count_dump::increment(const std::string &tag) {
    add(tag, 1);
  }

  void count_dump::decrement(const std::string &tag) {
    del(tag, 1);
  }

  void count_dump::reset(const std::string &tag) {
    chef::lock_guard<chef::mutex> guard(mutex_);
    tag2num_[tag] = 0;
  }

  void count_dump::reset() {
    chef::lock_guard<chef::mutex> guard(mutex_);
//    for (auto &iter : tag2num_) {
//      iter.second = 0;
//    }
    tag2num_iterator iter = tag2num_.begin();
    for (; iter != tag2num_.end(); iter++) {
        iter->second = 0;
    }
  }

  void count_dump::run_in_thread_() {
    while(!exit_flag_) {
      std::stringstream ss;

      std::time_t now = std::time(NULL);
      ss << "dump - " << std::asctime(std::localtime(&now)) << "-----\n";

      { /// lock
        chef::lock_guard<chef::mutex> guard(mutex_);
//        auto iter = tag2num_.begin();
        tag2num_iterator iter = tag2num_.begin();
        uint32_t count = 1;
        for (; iter != tag2num_.end(); iter++, count++) {
          ss << iter->first << ": " << iter->second;
          if ((count % NUM_OF_TAG_PER_LINE == 0) ||
            count == tag2num_.size()
          ) {
            ss << "\n";
          } else {
            ss << " | ";
          }
        }
      } /// unlock
      ss << "\n";
      filepath_op::write_file(filename_+".tmp", ss.str());
      filepath_op::rename(filename_+".tmp", filename_);

      chef::this_thread::sleep_for(chef::chrono::microseconds(dump_interval_ms_));
    }
  }

  std::map<std::string, int> count_dump::kv() {
    chef::lock_guard<chef::mutex> guard(mutex_);
    return tag2num_;
  }

} // namespace chef
