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
 * @NOTICE 实现参考了zlib的官方例子 `zlib/example/zpipe.c`
 *
 */

#ifndef _CHEF_BASE_COMPRESS_ZLIB_OP_H_
#define _CHEF_BASE_COMPRESS_ZLIB_OP_H_

namespace chef {

  class compress_zlib_op {
    public:
      enum COMPRESSION_LEVEL {
        COMPRESSION_LEVEL_DEFAULT          = -1, /// 默认
        COMPRESSION_LEVEL_BEST_SPEED       =  1, /// 最快
        COMPRESSION_LEVEL_BEST_COMPRESSION =  9, /// 压缩率最高
      };

    public:
      /**
       * zlib压缩
       *
       * @param      in 待压缩的数据
       * @param  in_len 待压缩数据的长度
       * @param     out 传出参数，压缩后的数据（内存由内部申请，外部用完后手动调用free释放）
       * @param out_len 传出参数，压缩后数据的长度
       * @param   level 压缩率，取值参见enum COMPRESSION_LEVEL
       *
       * @return 0 成功 -1 失败
       */
      int compress(const unsigned char *in, int in_len, unsigned char **out, int *out_len, COMPRESSION_LEVEL level);

      /**
       * zlib解压
       *
       * @param      in 待解压的数据
       * @param  in_len 待解压数据的长度
       * @param     out 传出参数，解压后的数据（内存由内部申请，外部用完后手动调用free释放）
       * @param out_len 传出参数，解压后数据的长度
       *
       * @return 0 成功 -1 失败
       *
       */
      int decompress(const unsigned char *in, int in_len, unsigned char **out, int *out_len);
  };

  /// TODO class compress_gzip_op

} /// namespace chef

#endif /// _CHEF_BASE_COMPRESS_ZLIB_OP_H_

