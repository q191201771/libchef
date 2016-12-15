/**
 * @file   compress_zlib_op.h/compress_zlib_op.cc
 * @deps   zlib
 *
 * @author
 *   chef <191201771@qq.com>
 *     -initial release xxxx-xx-xx
 *
 * @brief
 *   压缩、解压zlib流
 *
 */

#ifndef _CHEF_BASE_COMPRESS_ZLIB_OP_H_
#define _CHEF_BASE_COMPRESS_ZLIB_OP_H_

#include <string>

namespace chef {

  class compress_zlib_op {
    public:
      int compress(const std::string &in, std::string &out);

      int uncompress(const std::string &in, std::string &out);
  };

  class compress_gzip_op {
    public:
      int compress(const std::string &in, std::string &out);

      int uncompress(const std::string &in, std::string &out);
  };

} /// namespace chef

#endif /// _CHEF_BASE_COMPRESS_ZLIB_OP_H_
