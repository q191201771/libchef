#include "chef_count_dump.h"
#include "chef_filepath_op.h"
#include <assert.h>
#include <ctime>
#include <sstream>

namespace chef {

  #define LOCK_IF_NEEDED(t, m) if (t == MULTI_TAG_COUNTER_MUTEX) m.lock();
  #define UNLOCK_IF_NEEDED(t, m) if (t == MULTI_TAG_COUNTER_MUTEX) m.unlock();

  void multi_tag_counter::add_tag(const std::string &tag) {
    if (type_ == MULTI_TAG_COUNTER_ATOMIC) {
      tag2atomic_count_[tag] = 0;
      return;
    }
    LOCK_IF_NEEDED(type_, mutex_);
    tag2count_[tag] = 0;
    UNLOCK_IF_NEEDED(type_, mutex_);
  }

  void multi_tag_counter::add_tags(const std::vector<std::string> &tags) {
    std::vector<std::string>::const_iterator iter = tags.begin();
    for (; iter != tags.end(); iter++) {
      add_tag(*iter);
    }
  }

  void multi_tag_counter::del_tag(const std::string &tag) {
    if (type_ == MULTI_TAG_COUNTER_ATOMIC) {
      assert(0);
      tag2atomic_count_.erase(tag);
      return;
    }
    LOCK_IF_NEEDED(type_, mutex_);
    tag2count_.erase(tag);
    UNLOCK_IF_NEEDED(type_, mutex_);
  }

  bool multi_tag_counter::increment(const std::string &tag) {
    if (type_ == MULTI_TAG_COUNTER_ATOMIC) {
      TAG2ATOMIC_COUNT::iterator iter = tag2atomic_count_.find(tag);
      if (iter == tag2atomic_count_.end()) {
        return false;
      }
      iter->second++;
      return true;
    }
    LOCK_IF_NEEDED(type_, mutex_);
    tag2count_[tag]++;
    UNLOCK_IF_NEEDED(type_, mutex_);
    return true;
  }

  bool multi_tag_counter::decrement(const std::string &tag) {
    if (type_ == MULTI_TAG_COUNTER_ATOMIC) {
      TAG2ATOMIC_COUNT::iterator iter = tag2atomic_count_.find(tag);
      if (iter == tag2atomic_count_.end()) {
        return false;
      }
      iter->second--;
      return true;
    }
    LOCK_IF_NEEDED(type_, mutex_);
    tag2count_[tag]--;
    UNLOCK_IF_NEEDED(type_, mutex_);
    return true;
  }

  bool multi_tag_counter::add_count(const std::string &tag, int64_t num) {
    if (type_ == MULTI_TAG_COUNTER_ATOMIC) {
      TAG2ATOMIC_COUNT::iterator iter = tag2atomic_count_.find(tag);
      if (iter == tag2atomic_count_.end()) {
        return false;
      }
      iter->second += num;
      return true;
    }
    LOCK_IF_NEEDED(type_, mutex_);
    tag2count_[tag] += num;
    UNLOCK_IF_NEEDED(type_, mutex_);
    return true;
  }

  bool multi_tag_counter::del_count(const std::string &tag, int64_t num) {
    if (type_ == MULTI_TAG_COUNTER_ATOMIC) {
      TAG2ATOMIC_COUNT::iterator iter = tag2atomic_count_.find(tag);
      if (iter == tag2atomic_count_.end()) {
        return false;
      }
      iter->second -= num;
      return true;
    }
    LOCK_IF_NEEDED(type_, mutex_);
    tag2count_[tag] -= num;
    UNLOCK_IF_NEEDED(type_, mutex_);
    return true;
  }

  bool multi_tag_counter::set_count(const std::string &tag, int64_t num) {
    if (type_ == MULTI_TAG_COUNTER_ATOMIC) {
      TAG2ATOMIC_COUNT::iterator iter = tag2atomic_count_.find(tag);
      if (iter == tag2atomic_count_.end()) {
        return false;
      }
      iter->second = num;
      return true;
    }
    LOCK_IF_NEEDED(type_, mutex_);
    tag2count_[tag] = num;
    UNLOCK_IF_NEEDED(type_, mutex_);
    return true;
  }

  bool multi_tag_counter::get_tag_count(const std::string &tag, int64_t *num) {
    if (type_ == MULTI_TAG_COUNTER_ATOMIC) {
      TAG2ATOMIC_COUNT::iterator iter = tag2atomic_count_.find(tag);
      if (iter == tag2atomic_count_.end()) {
        return false;
      }
      *num = iter->second;
      return true;
    }
    LOCK_IF_NEEDED(type_, mutex_);
    bool ret = false;
    TAG2COUNT::iterator iter = tag2count_.find(tag);
    if (iter != tag2count_.end()) {
      *num = iter->second;
      ret = true;
    }
    UNLOCK_IF_NEEDED(type_, mutex_);
    return ret;
  }

  std::map<std::string, int64_t> multi_tag_counter::get_tags_count() {
    TAG2COUNT ret;
    if (type_ == MULTI_TAG_COUNTER_ATOMIC) {
      TAG2ATOMIC_COUNT::iterator iter = tag2atomic_count_.begin();
      for (; iter != tag2atomic_count_.end(); iter++) {
        ret[iter->first] = iter->second;
      }
      return ret;
    }
    LOCK_IF_NEEDED(type_, mutex_);
    ret = tag2count_;
    UNLOCK_IF_NEEDED(type_, mutex_);
    return ret;
  }

  multi_tag_count_dumper::~multi_tag_count_dumper() {
    exit_flag_ = true;
    if (thread_) {
      thread_->join();
      thread_.reset();
      dump2disk();
    }
  }

  void multi_tag_count_dumper::start() {
    thread_ = chef::make_shared<chef::thread>(chef::bind(&multi_tag_count_dumper::run_in_thread, this));
  }

  void multi_tag_count_dumper::run_in_thread() {
    while (!exit_flag_) {
      dump2disk();
      chef::this_thread::sleep_for(chef::chrono::milliseconds(interval_ms_));
    }
  }

  void multi_tag_count_dumper::dump2disk() {
    std::ostringstream ss;

    std::time_t now = std::time(NULL);
    ss << "count dump - " << std::asctime(std::localtime(&now))
       << "-------------------------------------\n\n";

    ss << styled_stringify();

    ss << "\n";
    filepath_op::write_file(filename_+".tmp", ss.str());
    filepath_op::rename(filename_+".tmp", filename_);
  }

  std::string multi_tag_count_dumper::styled_stringify() {
    std::map<std::string, int64_t> tag2count = mtc_->get_tags_count();

    std::ostringstream ss;
    std::map<std::string, int64_t>::iterator iter = tag2count.begin();
    uint32_t count = 1;
    for (; iter != tag2count.end(); iter++, count++) {
      ss << iter->first << ": " << iter->second;
      if ((count % num_per_line_ == 0) ||
          (count == tag2count.size())
      ) {
        ss << "\n";
      } else {
        ss << " | ";
      }
    }
    return ss.str();
  }

} // namespace chef
