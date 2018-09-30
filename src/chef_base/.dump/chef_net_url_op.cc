#include "chef_net_url_op.h"

namespace chef {

  std::string net_url_op::query_escape(const std::string &s) {
    int space_count = 0;
    int hex_count = 0;

    std::string ret;
    for (std::string::const_iterator it = s.begin(); it != s.end(); it++) {
      if (*it == ' ') {
        ret += '+';
        continue;
      }

      switch (*it) {
        case '$':
        case '&':
        case '+':
        case ',':
        case '/':
        case ';':
        case '=':
        case '?':
        case '@':
          ret += 
      }
    }
  }

} // namespace chef
