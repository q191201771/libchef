#include "filepath_op.h"
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <string.h>
#include <stdio.h>

#define IF_NULL_RETURN_MINUS_ONE(x) do { if ((x) == NULL) return -1; } while(0);

namespace chef {

  int filepath_op::exist(const char *name) {
    IF_NULL_RETURN_MINUS_ONE(name);
    struct stat st;
    return stat(name, &st);
  }

  int filepath_op::is_dir(const char *pathname) {
    IF_NULL_RETURN_MINUS_ONE(pathname);
    struct stat st;
    if (stat(pathname, &st) == -1) {
      return -1;
    }
    return S_ISDIR(st.st_mode) ? 0 : -1;
  }

  int filepath_op::mkdir_recursive(const char *pathname) {
    IF_NULL_RETURN_MINUS_ONE(pathname);
    char *path_dup = strdup(pathname);
    int len = strlen(path_dup);
    if (len == 0) {
        return -1;
    }
    int i = path_dup[0] == '/' ? 1 : 0;
    for (; i <= len; ++i) {
        if (path_dup[i] == '/' || path_dup[i] == '\0') {
            char ch = path_dup[i];
            path_dup[i] = '\0';
            if (::mkdir(path_dup, 0755) == -1 && errno != EEXIST) {
                free(path_dup);
                return -1;
            }
            path_dup[i] = ch;
        }
    }
    free(path_dup);
    return 0;
  }

  int filepath_op::rm_file(const char *name) {
    IF_NULL_RETURN_MINUS_ONE(name);
    if (exist(name) == -1) {
      return 0;
    }
    if (is_dir(name) == 0) {
      return -1;
    }
    if (::unlink(name) == -1) {
      return -1;
    }
    return 0;
  }

  int filepath_op::rm_dir(const char *pathname) {
    IF_NULL_RETURN_MINUS_ONE(pathname);
    if (exist(pathname) == -1) {
      return 0;
    }
    if (is_dir(pathname) == -1) {
      return -1;
    }
    return ::rmdir(pathname);
  }

  int filepath_op::rename(const char *src, const char *dst) {
    IF_NULL_RETURN_MINUS_ONE(src);
    IF_NULL_RETURN_MINUS_ONE(dst);
    return ::rename(src, dst);
  }

  int filepath_op::get_file_size(const char *filename) {
    IF_NULL_RETURN_MINUS_ONE(filename);
    if (exist(filename) == -1 || is_dir(filename) == 0) {
      return -1;
    }
    struct stat st;
    if (::stat(filename, &st) == -1) {
      return -1;
    }
    return (int)st.st_size;
  }

  int filepath_op::write_file(const char *filename, const char *content, int len) {
    if (filename == NULL || content == NULL || len <= 0) {
      return -1;
    }
    FILE *fp = fopen(filename, "wb");
    if (fp == NULL) {
      return -1;
    }
    int written = fwrite((const void *)content, len, 1, fp);
    fclose(fp);
    return written == len;
  }

  int filepath_op::read_file(const char *filename, char *content, int content_size) {
    if (filename == NULL || content == NULL || content_size <= 0) {
      return -1;
    }
    FILE *fp = fopen(filename, "rb");
    if (fp == NULL) {
      return -1;
    }
    int ret = fread((void *)content, content_size, 1, fp);
    fclose(fp);
    if (ret != 1) {
      return -1;
    }
    return 0;
  }

  std::string filepath_op::read_file(const std::string &filename) {
    int size = get_file_size(filename.c_str());
    if (size == -1) {
      return std::string();
    }
    char *content = new char[size];
    int ret = read_file(filename.c_str(), content, size);
    if (ret == -1) {
      delete []content;
      return std::string();
    }
    std::string content_string(content, size);
    delete []content;
    return content_string;
  }

} // namespace chef
