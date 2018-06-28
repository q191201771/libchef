/**
 * @file     chef_filepath_op.h[.cc]
 * @deps     nope
 * @platform linux/macos/xxx
 *
 * @author
 *   chef <191201771@qq.com>
 *     -initial release xxxx-xx-xx
 *
 * @brief  文件、文件夹操作相关
 *
 */

#ifndef _CHEF_BASE_FILEPATH_OP_H_
#define _CHEF_BASE_FILEPATH_OP_H_
#pragma once

#include <string>
#include <vector>
#include <stdint.h>

namespace chef {

  class filepath_op {
    public:
      /**
       * @return
       *    0 存在，文件或文件夹都可
       *   -1 不存在
       *
       */
      static int exist(const std::string &name);

      /**
       * @return
       *    0 存在，且为文件夹
       *   -1 不存在，或不是文件夹
       *
       */
      static int is_dir(const std::string &pathname);

      /**
       * @param pathname             需要查询的文件夹
       * @param child_dirs           传出参数，文件夹下的文件夹
       * @param child_files          传出参数，文件夹下的文件
       * @param with_pathname_prefix 传出的文件、文件夹前是否加上`pathname`前缀
       *
       * @return  0 成功 -1 失败 `pathname`不可遍历
       *
       * @NOTICE 只访问第一层目录，不会递归访问子目录下的内容
       *
       */
      static int walk_dir(const std::string &pathname,
                          std::vector<std::string> &child_dirs /*out*/,
                          std::vector<std::string> &child_files /*out*/,
                          bool with_pathname_prefix=true);

      /**
       * @return
       *    0 创建成功，或创建前已经存在
       *   -1 失败
       *
       */
      static int mkdir_recursive(const std::string &pathname);

      /**
       * @param name 文件名
       *
       * @return
       *    0 删除成功，或删除前就不存在
       *   -1 删除失败，或`name`是文件夹
       *
       */
      static int rm_file(const std::string &name);

      /**
       * @param pathname 文件夹名
       *
       * @return
       *    0 删除成功，或删除前就不存在
       *   -1 删除失败，或`pathname`不是文件夹
       *
       */
      static int rmdir_recursive(const std::string &pathname);

      /**
       * @param src 源文件
       * @param dst 目标文件
       *
       * @return 0 成功 -1 失败
       *
       */
      static int rename(const std::string &src, const std::string &dst);

      /**
       * 写文件
       *
       * @return 0 成功 -1 失败
       *
       */
      static int write_file(const std::string &filename, const std::string &content);

      /**
       * 写文件
       *
       * @return 0 成功 -1 失败
       *
       */
      static int write_file(const std::string &filename, const char *content, size_t content_size);

      /**
       * @NOTICE
       * For most files under the /proc directory, stat() does not return the file
       * size in the st_size field; instead the field is returned with the value 0.
       *
       * @return 文件大小，失败则返回-1
       *
       */
      static int64_t get_file_size(const std::string &filename);

      /**
       * 读文件，对get_file_size()+read_file()的封装，更易于使用
       *
       * @return 成功返回文件内容，失败返回std::string()
       *
       */
      static std::string read_file(const std::string &filename);

      /**
       * 由于/proc下面的文件无法通过::stat()获取文件长度，所以提供参数让调用者填入一个fixed长度
       *
       */
      static std::string read_file(const std::string &filename, size_t content_size);

      /**
       * @param filename     文件名
       * @param content      传出参数，读取到的文件内容，内存由外部申请
       * @param content_size 最大读入大小
       *
       * @return 成功返回实际读入大小，失败返回-1
       *
       */
      static int64_t read_file(const std::string &filename, char *content /*out*/, size_t content_size);

      /// @TODO 能否统一成一个接口，内部判断是否是否为link
      static std::string read_link(const std::string &filename, size_t content_size);

      /**
       * @param path     目录
       * @param filename 文件名
       *
       * 连接目录和文件名，解决`path`后面'/'和`filename`前面'/'是否存在，重复的问题
       *
       */
      static std::string join(const std::string &path, const std::string &filename);
  };

} // namespace chef

#endif // _CHEF_BASE_FILEPATH_OP_H_
