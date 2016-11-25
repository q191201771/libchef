#include "filepath_op.h"
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <dirent.h>
#include <errno.h>

#define IF_ZERO_RETURN_NAGETIVE_ONE(x) do { if ((x) == 0) return -1; } while(0);
#define IF_NULL_RETURN_NAGETIVE_ONE(x) do { if ((x) == NULL) return -1; } while(0);
#define IF_STRING_EMPTY_RETURN_NAGETIVE_ONE(x) do { if (x.length() == 0) return -1; } while(0);

namespace chef {

  int filepath_op::exist(const std::string &name) {
    IF_STRING_EMPTY_RETURN_NAGETIVE_ONE(name);
    struct stat st;
    return stat(name.c_str(), &st);
  }

  int filepath_op::is_dir(const std::string &pathname) {
    IF_STRING_EMPTY_RETURN_NAGETIVE_ONE(pathname);
    struct stat st;
    if (stat(pathname.c_str(), &st) == -1) {
      return -1;
    }
    return S_ISDIR(st.st_mode) ? 0 : -1;
  }

  int filepath_op::mkdir_recursive(const std::string &pathname) {
    IF_STRING_EMPTY_RETURN_NAGETIVE_ONE(pathname);
    char *path_dup = strdup(pathname.c_str());
    size_t len = strlen(path_dup);
    if (len == 0) {
        return -1;
    }
    size_t i = path_dup[0] == '/' ? 1 : 0;
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

  int filepath_op::rm_file(const std::string &name) {
    IF_STRING_EMPTY_RETURN_NAGETIVE_ONE(name);
    if (exist(name) == -1) {
      return 0;
    }
    if (is_dir(name) == 0) {
      return -1;
    }
    if (::unlink(name.c_str()) == -1) {
      return -1;
    }
    return 0;
  }

  int filepath_op::rmdir_recursive(const std::string &pathname) {
    IF_STRING_EMPTY_RETURN_NAGETIVE_ONE(pathname);
    if (exist(pathname) == -1) {
      return 0;
    }
    if (is_dir(pathname) == -1) {
      return -1;
    }

    DIR *open_ret = ::opendir(pathname.c_str());
    IF_NULL_RETURN_NAGETIVE_ONE(open_ret);

    struct dirent entry;
    struct dirent *result = NULL;
    int ret = 0;
    for (;;) {
      if (::readdir_r(open_ret, &entry, &result) != 0) {
        break;
      }
      if (result == NULL) {
        break;
      }
      char *name = result->d_name;
      if (strcmp(name, ".") == 0 ||
        strcmp(name, "..") == 0
      ) {
        continue;
      }
      std::string file_with_path = filepath_op::join(pathname, name);
      if (filepath_op::exist(file_with_path.c_str()) != 0) {
        fprintf(stderr, "%s:%d %s\n", __FUNCTION__, __LINE__, file_with_path.c_str());
        continue;
      }
      if (filepath_op::is_dir(file_with_path.c_str()) == 0) {
        if (filepath_op::rmdir_recursive(file_with_path.c_str()) != 0) {
          ret = -1;
        }
      } else {
        if (filepath_op::rm_file(file_with_path.c_str()) != 0) {
          ret = -1;
        }
      }
    }

    if (open_ret) {
      ::closedir(open_ret);
    }

    return (::rmdir(pathname.c_str()) == 0 && ret == 0) ? 0 : -1;
  }

  int filepath_op::rename(const std::string &src, const std::string &dst) {
    IF_STRING_EMPTY_RETURN_NAGETIVE_ONE(src);
    IF_STRING_EMPTY_RETURN_NAGETIVE_ONE(dst);
    return ::rename(src.c_str(), dst.c_str());
  }

  int filepath_op::write_file(const std::string &filename, const char *content, size_t content_size) {
    IF_STRING_EMPTY_RETURN_NAGETIVE_ONE(filename);
    IF_NULL_RETURN_NAGETIVE_ONE(content);
    IF_ZERO_RETURN_NAGETIVE_ONE(content_size);
    FILE *fp = fopen(filename.c_str(), "wb");
    IF_NULL_RETURN_NAGETIVE_ONE(fp);
    size_t written = fwrite(reinterpret_cast<const void *>(content), content_size, 1, fp);
    fclose(fp);
    return written == content_size;
  }

  int filepath_op::write_file(const std::string &filename, const std::string &content) {
    return filepath_op::write_file(filename, content.c_str(), content.length());
  }

  int64_t filepath_op::get_file_size(const std::string &filename) {
    IF_STRING_EMPTY_RETURN_NAGETIVE_ONE(filename);
    if (exist(filename) == -1 || is_dir(filename) == 0) {
      return -1;
    }
    struct stat st;
    if (::stat(filename.c_str(), &st) == -1) {
      return -1;
    }
    return st.st_size;
  }

  int64_t filepath_op::read_file(const std::string &filename, char *content, size_t content_size) {
    IF_STRING_EMPTY_RETURN_NAGETIVE_ONE(filename);
    IF_NULL_RETURN_NAGETIVE_ONE(content);
    IF_ZERO_RETURN_NAGETIVE_ONE(content_size);
    FILE *fp = fopen(filename.c_str(), "rb");
    IF_NULL_RETURN_NAGETIVE_ONE(fp);
    size_t read_size = fread(reinterpret_cast<void *>(content), 1, content_size, fp);
    fclose(fp);
    return read_size;
  }

  std::string filepath_op::read_file(const std::string &filename) {
    int64_t size = get_file_size(filename.c_str());
    if (size <= 0) {
      return std::string();
    }
    return read_file(filename, size);
  }

  std::string filepath_op::read_file(const std::string &filename, size_t content_size) {
    if (content_size == 0) {
      return std::string();
    }
    char *content = new char[content_size];
    int64_t read_size = read_file(filename.c_str(), content, content_size);
    if (read_size == -1) {
      delete []content;
      return std::string();
    }
    std::string content_string(content, read_size);
    delete []content;
    return content_string;
  }

  std::string filepath_op::read_link(const std::string &filename, size_t content_size) {
    if (filename.length() == 0 || content_size == 0) {
      return std::string();
    }
    char *content = new char[content_size];
    ssize_t length = ::readlink(filename.c_str(), content, content_size);
    if (length == -1) {
      delete []content;
      return std::string();
    }
    std::string ret(content, length);
    delete []content;
    return ret;
  }

  std::string filepath_op::join(const std::string &path, const std::string &filename) {
    std::string ret;
    size_t path_length = path.length();
    size_t filename_length = filename.length();
    if (path_length == 0) {
      return filename;
    }
    if (filename_length == 0) {
      return path;
    }
    if (path[path_length - 1] == '/') {
      ret = path.substr(0, path_length - 1);
    } else {
      ret = path;
    }
    ret += "/";
    if (filename[0] == '/') {
      ret += filename.substr(1, filename_length - 1);
    } else {
      ret += filename;
    }
    return ret;
  }

} // namespace chef
