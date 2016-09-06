#include "this_machine_op.h"
#include "filepath_op.h"
#include <sys/sysinfo.h>
#include <sstream>
#include <ctime>
#include <cstdlib>
#include <cstring>

namespace inner {

  class netdev {
    public:
      netdev()
        : in_bytes_(0)
        , out_bytes_(0)
      {}

      int parse(const std::string &interface) {
        std::string content = chef::filepath_op::read_file("/proc/net/dev", 65535);
        if (content.empty()) {
          return -1;
        }
        std::string key = interface + ": ";
        size_t pos = content.find(key);
        if (pos == std::string::npos) {
          return -1;
        }
        std::istringstream ss(std::string(content.c_str() + pos + key.length()));
        int dummy;
        ss >> in_bytes_ >> dummy >> dummy >> dummy >> dummy >> dummy >> dummy >> dummy
          >> out_bytes_;
        return 0;
      }

      uint64_t in_bytes_;
      uint64_t out_bytes_;
  };

  class meminfo {
    public:
      meminfo()
        : mem_total_kb_(0)
        , mem_free_kb_(0)
        , mem_buffers_kb_(0)
        , mem_cached_kb_(0)
        , mem_used_kb_(0)
      {}

      int mem_total_kb_;
      int mem_free_kb_;
      int mem_buffers_kb_;
      int mem_cached_kb_;
      int mem_used_kb_;

      int parse() {
        std::string content = chef::filepath_op::read_file("/proc/meminfo", 65535);
        if (content.empty()) {
          return -1;
        }
        mem_total_kb_   = parse_int(content, "MemTotal:");
        mem_free_kb_    = parse_int(content, "MemFree:");
        mem_buffers_kb_ = parse_int(content, "Buffers:");
        mem_cached_kb_  = parse_int(content, "Cached:");
        mem_used_kb_    = mem_total_kb_ - mem_free_kb_ - mem_buffers_kb_ - mem_cached_kb_;
        return 0;
      }
    private:
      int parse_int(const std::string &content, const std::string &key) {
        size_t pos = content.find(key);
        if (pos == std::string::npos) {
          return 0;
        }
        return std::atoi(content.c_str() + pos + key.length());
      }
  };

  class stat {
    public:
      stat() : btime_(0) {}

      int btime_;

      int parse() {
        std::string content = chef::filepath_op::read_file("/proc/stat", 65535);
        if (content.empty()) {
          return -1;
        }
        size_t pos = content.find("btime");
        if (pos == std::string::npos) {
          return -1;
        }
        btime_ = std::atoi(content.c_str() + pos + strlen("btime"));
        return 0;
      }
  };

} // namespace inner

namespace chef {

  int this_machine_op::num_of_available_cpu_cores() {
    return ::get_nprocs();
  }

  int this_machine_op::mem_total_kb() {
    inner::meminfo mi;
    mi.parse();
    return mi.mem_total_kb_;
  }

  int this_machine_op::mem_free_kb() {
    inner::meminfo mi;
    mi.parse();
    return mi.mem_free_kb_;
  }

  int this_machine_op::mem_buffers_kb() {
    inner::meminfo mi;
    mi.parse();
    return mi.mem_buffers_kb_;
  }

  int this_machine_op::mem_cached_kb() {
    inner::meminfo mi;
    mi.parse();
    return mi.mem_cached_kb_;
  }

  int this_machine_op::mem_used_kb() {
    inner::meminfo mi;
    mi.parse();
    return mi.mem_used_kb_;
  }

  int this_machine_op::boot_timestamp() {
    inner::stat st;
    st.parse();
    return st.btime_;
  }

  int this_machine_op::up_duration_seconds() {
    inner::stat st;
    if (st.parse() == -1) {
      return 0;
    }
    return (int)std::time(NULL) - st.btime_;
  }

  int this_machine_op::io_bytes(const std::string &interface, uint64_t *in, uint64_t *out) {
    if (in == NULL || out == NULL) {
      return -1;
    }
    inner::netdev nd;
    if (nd.parse(interface) == -1) {
      return -1;
    }
    *in = nd.in_bytes_;
    *out = nd.out_bytes_;
    return 0;
  }

} // namespace chef
