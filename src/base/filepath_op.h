/**
 * @author chef <191201771@qq.com>
 * @deps   nope
 */

#ifndef _CHEF_BASE_FILEPATH_OP_H_
#define _CHEF_BASE_FILEPATH_OP_H_

#include <string>

namespace chef {

  class filepath_op {
    public:
      /**
       * @return
       * 0  成功(存在，文件或文件夹都可)
       * -1 失败
       */
      static int exist(const char *name);

      /**
       * @return
       * 0  成功(存在，且为文件夹)
       * -1 失败
       */
      static int is_dir(const char *pathname);

      /**
       * @return
       * 0  成功(创建成功，或创建前已经存在)
       * -1 失败
       */
      static int mkdir_recursive(const char *pathname);

      /**
       * @param name 文件名
       *
       * @return
       * 0  成功(删除成功，或删除前就不存在)
       * -1 失败(删除失败，或[name]是文件夹)
       */
      static int rm_file(const char *name);

      /**
       * TODO 如果目录中有文件，删除会失败!
       *
       * @param name 文件夹名
       *
       * @return
       * 0  成功(删除成功，或删除前就不存在)
       * -1 失败(删除失败，或[name]不是文件夹)
       */
      static int rm_dir(const char *pathname);

      /**
       * @param
       * src 源文件
       * dst 目标文件
       *
       * @return 0 成功 -1 失败
       */
      static int rename(const char *src, const char *dst);

      /**
       * @return 文件大小,失败则返回-1
       */
      static int get_file_size(const char *filename);

      /**
       * 写文件
       *
       * @return 0 成功 -1 失败
       */
      static int write_file(const char *filename, const char *content, int len);

      /**
       * 读文件,对get_file_size+read_file的封装,更易于使用
       *
       * @return 成功返回文件内容，失败返回std::string()
       */
      static std::string read_file(const std::string &filename);

    private:
      /**
       * 读文件,使用前需调用get_file_size获取文件内容大小
       *
       * @param
       * filename     文件名
       * content      传出参数,读取到的文件内容,内存由外部申请
       * content_size 文件内容大小
       *
       * @return 0 成功 -1 失败
       */
      static int read_file(const char *filename, char *content/* out */, int content_size);
  };

} // namespace chef

#endif // _CHEF_BASE_FILEPATH_OP_H_
