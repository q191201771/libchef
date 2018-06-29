#include "chef_env.hpp"
#include <unistd.h>
#include <pthread.h>
#include <sys/types.h>
#include <sys/syscall.h>

BOOST_LOG_INLINE_GLOBAL_LOGGER_DEFAULT(global_logger_src,
                                       boost::log::sources::severity_logger_mt<boost::log::trivial::severity_level>);

static inline int _threadid() {
#if defined(__linux__)
  return static_cast<int>(syscall(SYS_gettid));
#elif defined(__MACH__)
  uint64_t tid64;
  pthread_threadid_np(NULL, &tid64);
  return static_cast<int>(tid64);
#else
  return 0;
#endif
}

namespace chef {

  inline boost::log::trivial::severity_level &log::get_level() {
    static boost::log::trivial::severity_level level_ = boost::log::trivial::trace;
    return level_;
  }

  inline void log::init(mode m, const std::string &filename_with_path) {
    boost::shared_ptr<boost::log::core> core = boost::log::core::get();
    core->add_global_attribute("TimeStamp", boost::log::attributes::local_clock());
    core->add_global_attribute("ThreadID", boost::log::attributes::make_function(&_threadid));
    core->add_global_attribute("Scope", boost::log::attributes::named_scope());
    core->set_exception_handler(boost::log::make_exception_suppressor());

    // format
    boost::log::formatter formatter;
    if (m == mode_debug) {
      formatter = boost::log::expressions::stream <<
          "[" << boost::log::expressions::format_date_time<boost::posix_time::ptime>("TimeStamp", "%Y-%m-%d %H:%M:%S.%f") <<
          //"] [" << boost::log::expressions::attr<boost::log::attributes::current_thread_id::value_type>("ThreadID") <<
          "] [" << boost::log::expressions::attr<int>("ThreadID") <<
          "] [" << boost::log::expressions::attr<boost::log::trivial::severity_level>("Severity") <<
          "] [" << boost::log::expressions::format_named_scope("Scope", boost::log::keywords::format = "%f:%l",
                                                               boost::log::keywords::depth = 1) <<
          "] | "<< boost::log::expressions::smessage;
    } else {
      formatter = boost::log::expressions::stream <<
          boost::log::expressions::format_date_time<boost::posix_time::ptime>("TimeStamp", "%Y-%m-%d %H:%M:%S") << " " <<
          std::setw(5) << boost::log::expressions::attr<pid_t>("ThreadID") << " " <<
          std::setw(7) << boost::log::expressions::attr<boost::log::trivial::severity_level>("Severity") << " | " <<
          boost::log::expressions::smessage;
    }

    // filename
    std::string filename;
    if (!filename_with_path.empty()) {
      filename = filename_with_path;
    } else {
      char pc_name_buf[128] = {0};
      std::string pc_name = "unknown";
      if (gethostname(pc_name_buf, 128) != -1) {
        pc_name = pc_name_buf;
      }
      std::string bin_name = "unknown";
      char buf[512] = {0};
      ssize_t len = readlink("/proc/self/exe", buf, 511);
      if (len > 0 && len < 511) {
        bin_name.assign(buf, len);
        std::size_t pos = bin_name.find_last_of('/');
        if (pos != std::string::npos) {
          bin_name.substr(0, pos+1);
        }
      }
      char pid_buf[16] = {0};
      snprintf(pid_buf, 15, "%d", getpid());

      filename = bin_name+".%Y%m%dT%H%M%S.%N."+pc_name+"."+pid_buf+".log.chef";
    }

    boost::shared_ptr<boost::log::sinks::text_file_backend> backend = boost::make_shared<boost::log::sinks::text_file_backend>(
        boost::log::keywords::file_name = filename,
        boost::log::keywords::rotation_size = 1024ULL * 1024 * 1024,
        boost::log::keywords::time_based_rotation = boost::log::sinks::file::rotation_at_time_point(0, 0, 0),
        boost::log::keywords::open_mode = std::ios::app,
        boost::log::keywords::auto_flush = (m == mode_debug));

    get_level() = (m == mode_debug) ? boost::log::trivial::trace : boost::log::trivial::info;

    typedef boost::log::sinks::synchronous_sink<boost::log::sinks::text_file_backend> sync_sink_frontend;
    boost::shared_ptr<sync_sink_frontend> frontend(new sync_sink_frontend(backend));
    frontend->set_filter(boost::log::expressions::attr<boost::log::trivial::severity_level>("Severity") >= get_level());
    frontend->set_formatter(formatter);

    core->add_sink(frontend);

    if (m == mode_release) {
      chef::thread t([backend] (){
        for (; ; ) {
          backend->flush();
          usleep(1000 * 1000);
        }
      });
      t.detach();
    }

    CHEF_LOG(trace)   << "Engine by boost::log,I'm trace-severity.";
    CHEF_LOG(debug)   << "Engine by boost::log,I'm debug-severity.";
    CHEF_LOG(info)    << "Engine by boost::log,I'm info-severity.";
    CHEF_LOG(warning) << "Engine by boost::log,I'm warning-severity.";
    CHEF_LOG(error)   << "Engine by boost::log,I'm error-severity.";
    CHEF_LOG(fatal)   << "Engine by boost::log,I'm fatal-severity.";
  }

  inline void log::force_flush() {
    boost::shared_ptr<boost::log::core> core = boost::log::core::get();
    core->flush();
  }

  namespace internal {
    inline boost::log::sources::severity_logger_mt<boost::log::trivial::severity_level> &get_logger() {
      return global_logger_src::get();
    }
  } // namespace internal

} // namespace chef
