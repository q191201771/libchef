#include "chef_count_dump.h"
#include "chef_filepath_op.h"
#include <assert.h>
#include <ctime>
#include <sstream>

namespace chef {

  count_dump::count_dump()
    : type_(COUNT_DUMP_TYPE_IMMUTABLE_TAGS)
    , filename_(std::string())
    , dump_interval_ms_(-1)
    , num_of_tag_per_line_(-1)
    , exit_flag_(false)
  {}

  count_dump::~count_dump() {
    exit_flag_ = true;
    if (thread_) {
      thread_->join();
      thread_.reset();
      dump2disk_();
    }
  }

  void count_dump::init(const std::string &filename, const count_dump_config &config) {
    type_                = COUNT_DUMP_TYPE_MUTABLE_TAGS;
    filename_            = filename;
    dump_interval_ms_    = config.dump_interval_ms_;
    num_of_tag_per_line_ = config.num_of_tag_per_line_;

    thread_ = chef::make_shared<chef::thread>(chef::bind(&count_dump::run_in_thread_, this));
  }

  void count_dump::init_with_constant_tags(const std::string &filename, const std::vector<std::string> &tags,
                                           const count_dump_config &config)
  {
    type_                = COUNT_DUMP_TYPE_IMMUTABLE_TAGS;
    filename_            = filename;
    dump_interval_ms_    = config.dump_interval_ms_;
    num_of_tag_per_line_ = config.num_of_tag_per_line_;

    std::vector<std::string>::const_iterator iter = tags.begin();
    for (; iter != tags.end(); iter++) {
      tag2atomic_num_[*iter] = chef::make_shared<chef::atomic<int> >(0);
    }

    thread_ = chef::make_shared<chef::thread>(chef::bind(&count_dump::run_in_thread_, this));
  }

  int count_dump::add(const std::string &tag, int num) {
    if (type_ == COUNT_DUMP_TYPE_MUTABLE_TAGS) {
      chef::lock_guard<chef::mutex> guard(mutex_);
      tag2num_iterator iter = tag2num_.find(tag);
      if (iter == tag2num_.end()) {
        tag2num_[tag] = num;
      } else {
        iter->second += num;
      }
    } else if (type_ == COUNT_DUMP_TYPE_IMMUTABLE_TAGS) {
      tag2atomic_num_iterator iter = tag2atomic_num_.find(tag);
      if (iter == tag2atomic_num_.end()) {
        return -1;
      }
      iter->second->fetch_add(num);
    }
    return 0;
  }

  int count_dump::del(const std::string &tag, int num) { return add(tag, -num); }

  int count_dump::increment(const std::string &tag) { return add(tag, 1); }

  int count_dump::decrement(const std::string &tag) { return del(tag, 1); }

  int count_dump::reset(const std::string &tag) {
    if (type_ == COUNT_DUMP_TYPE_MUTABLE_TAGS) {
      chef::lock_guard<chef::mutex> guard(mutex_);
      if (tag2num_.find(tag) == tag2num_.end()) {
        return -1;
      }
      tag2num_[tag] = 0;
    } else if (type_ == COUNT_DUMP_TYPE_IMMUTABLE_TAGS) {
      tag2atomic_num_iterator iter = tag2atomic_num_.find(tag);
      if (iter == tag2atomic_num_.end()) {
        return -1;
      }
      iter->second->store(0);
    }
    return 0;
  }

  void count_dump::reset() {
    if (type_ == COUNT_DUMP_TYPE_MUTABLE_TAGS) {
      chef::lock_guard<chef::mutex> guard(mutex_);
      tag2num_iterator iter = tag2num_.begin();
      for (; iter != tag2num_.end(); iter++) {
        iter->second = 0;
      }
    } else if (type_ == COUNT_DUMP_TYPE_IMMUTABLE_TAGS) {
      tag2atomic_num_iterator iter = tag2atomic_num_.begin();
      for (; iter != tag2atomic_num_.end(); iter++) {
        iter->second->store(0);
      }
    }
  }

  void count_dump::run_in_thread_() {
    for (; !exit_flag_; ) {
      dump2disk_();

      chef::this_thread::sleep_for(chef::chrono::milliseconds(dump_interval_ms_));
    }
  }

  void count_dump::dump2disk_() {
    std::ostringstream ss;

    std::time_t now = std::time(NULL);
    ss << "count dump - " << std::asctime(std::localtime(&now))
       << "-------------------------------------\n\n";

    ss << styled_stringify();

    ss << "\n";
    filepath_op::write_file(filename_+".tmp", ss.str());
    filepath_op::rename(filename_+".tmp", filename_);
  }

  std::map<std::string, int> count_dump::kvs() {
    std::map<std::string, int> ret;
    if (type_ == COUNT_DUMP_TYPE_MUTABLE_TAGS) {
      chef::lock_guard<chef::mutex> guard(mutex_);
      ret = tag2num_;
    } else if (type_ == COUNT_DUMP_TYPE_IMMUTABLE_TAGS) {
      tag2atomic_num_iterator iter = tag2atomic_num_.begin();
      for (; iter != tag2atomic_num_.end(); iter++) {
        ret[iter->first] = iter->second->load();
      }
    }
    return ret;
  }

  std::string count_dump::stringify() {
    std::ostringstream ss;
    ss << "{";
    tag2num tags = kvs();
    tag2num_iterator iter = tags.begin();
    uint32_t count = 1;
    for (; iter != tags.end(); iter++, count++) {
      ss << iter->first << ":" << iter->second;
      if (count != tags.size()) {
        ss << ",";
      }
    }
    ss << "}";

    return ss.str();
  }

  std::string count_dump::styled_stringify() {
    std::ostringstream ss;

    if (type_ == COUNT_DUMP_TYPE_MUTABLE_TAGS){
      chef::lock_guard<chef::mutex> guard(mutex_);
      tag2num_iterator iter = tag2num_.begin();
      uint32_t count = 1;
      for (; iter != tag2num_.end(); iter++, count++) {
        ss << iter->first << ": " << iter->second;
        if ((count % num_of_tag_per_line_ == 0) ||
            (count == tag2num_.size())
        ) {
          ss << "\n";
        } else {
          ss << " | ";
        }
      }
    } else if (type_ == COUNT_DUMP_TYPE_IMMUTABLE_TAGS) {
      tag2atomic_num_iterator iter = tag2atomic_num_.begin();
      uint32_t count = 1;
      for (; iter != tag2atomic_num_.end(); iter++, count++) {
        ss << iter->first << ": " << iter->second->load();
        if ((count % num_of_tag_per_line_ == 0) ||
            (count == tag2atomic_num_.size())
        ) {
          ss << "\n";
        } else {
          ss << " | ";
        }
      }
    }

    return ss.str();
  }

} // namespace chef
