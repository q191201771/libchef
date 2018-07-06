#include "os_exec_op.h"
#include <string.h>
#include <stdio.h>

namespace chef {

  std::vector<std::string> os_exec_op::run_command(const std::string &cmd) {
    std::vector<std::string> result;

    FILE *fp = popen(cmd.c_str(), "r");
    if (fp == NULL) {
      fprintf(stderr, "popen(\"%s\") fail.\n", cmd.c_str());
      return result;
    }

    const int MAX_LEN = 1024;
    char line[MAX_LEN] = {0};
    for (; fgets(line, MAX_LEN, fp); ) {
      size_t rend = strlen(line) - 1;
      if (line[rend] == '\n') {
        line[rend] = '\0';
      }
      result.push_back(line);
    }

    pclose(fp);

    return result;
  }

} // namespace chef
