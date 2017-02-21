#include "chef_log.h"
#include <unistd.h>
#include <sys/types.h>
#include <sys/syscall.h>

BOOST_LOG_INLINE_GLOBAL_LOGGER_DEFAULT(global_logger_src,
                                       boost::log::sources::severity_logger_mt<boost::log::trivial::severity_level>);

static pid_t gettid() {
  return static_cast<pid_t>(syscall(SYS_gettid));
}

namespace chef {

  void log::init(mode m)
  {
    boost::shared_ptr<boost::log::core> core = boost::log::core::get();
    core->add_global_attribute("TimeStamp", boost::log::attributes::local_clock());
    core->add_global_attribute("ProcessID", boost::log::attributes::make_function(&getpid));
    core->add_global_attribute("ThreadID", boost::log::attributes::make_function(&gettid));
    core->add_global_attribute("Scope", boost::log::attributes::named_scope());

    core->set_exception_handler(boost::log::make_exception_suppressor());

    typedef boost::log::sinks::synchronous_sink<boost::log::sinks::text_file_backend> sync_sink_frontend;

    boost::log::formatter formatter;
    if (m == mode_debug) {
      //boost::log::expressions::attr<pid_t>("ProcessID")
      formatter = boost::log::expressions::stream <<
          "[" << boost::log::expressions::format_date_time<boost::posix_time::ptime>("TimeStamp", "%Y-%m-%d %H:%M:%S.%f") <<
          "] [" << boost::log::expressions::attr<pid_t>("ThreadID") <<
          "] [" << boost::log::expressions::attr<boost::log::trivial::severity_level>("Severity") <<
          "] [" << boost::log::expressions::format_named_scope("Scope", boost::log::keywords::format = "%f:%l",
                                                               boost::log::keywords::depth = 1) <<
          "] | "<< boost::log::expressions::smessage;
    } else {
      formatter = boost::log::expressions::stream <<
          boost::log::expressions::format_date_time<boost::posix_time::ptime>("TimeStamp", "%Y-%m-%d %H:%M:%S.%f") << " " <<
          std::setw(5) << boost::log::expressions::attr<pid_t>("ThreadID") << " " <<
          std::setw(7) << boost::log::expressions::attr<boost::log::trivial::severity_level>("Severity") << " | " <<
          boost::log::expressions::smessage;
    }
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

    boost::shared_ptr<boost::log::sinks::text_file_backend> backend = boost::make_shared<boost::log::sinks::text_file_backend>(
        boost::log::keywords::file_name = bin_name+".%Y%m%dT%H%M%S.%N."+pc_name+"."+pid_buf+".log.chef",
        boost::log::keywords::rotation_size = 1024ULL * 1024 * 1024,
        boost::log::keywords::time_based_rotation = boost::log::sinks::file::rotation_at_time_point(0, 0, 0),
        boost::log::keywords::open_mode = std::ios::app,
        boost::log::keywords::auto_flush = true);

    boost::log::trivial::severity_level base_level = (m == mode_debug) ? boost::log::trivial::trace : boost::log::trivial::info;
    boost::shared_ptr<sync_sink_frontend> frontend(new sync_sink_frontend(backend));
    frontend->set_filter(boost::log::expressions::attr<boost::log::trivial::severity_level>("Severity") >= base_level);
    frontend->set_formatter(formatter);
    core->add_sink(frontend);

    CHEF_LOG(trace)   << "engine by boost::log,i'm trace-severity";
    CHEF_LOG(debug)   << "engine by boost::log,i'm debug-severity";
    CHEF_LOG(info)    << "engine by boost::log,i'm info-severity";
    CHEF_LOG(warning) << "engine by boost::log,i'm warning-severity";
    CHEF_LOG(error)   << "engine by boost::log,i'm error-severity";
    CHEF_LOG(fatal)   << "engine by boost::log,i'm fatal-severity";
  }

  namespace internal {
    boost::log::sources::severity_logger_mt<boost::log::trivial::severity_level> &get_logger() {
      return global_logger_src::get();
    }
  } // namespace internal

} // namespace chef
