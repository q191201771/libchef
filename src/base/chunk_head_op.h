/**
 * @file   chunk_head_op.h/chunk_head_op.cc
 * @deps   nope
 *
 * @author
 *   chef <191201771@qq.com>
 *     -initial release 2016-08-31
 *
 * @brief
 *   一个简易的Type-Length-Value实现，可用于tcp流数据组包、拆包。
 *   流格式为：chunk-head chunk-body chunk chunk-body ...
 *
 *   +--------+------+-----------+----------+----------+
 *   |   id   | type | magic num | reserved | body len | chunk body | ...
 *   0        8     12          16         20         23           23+body len
 *   ---------------------------------------------------
 *
 */

#ifndef _CHEF_BASE_CHUNK_HEAD_OP_H_
#define _CHEF_BASE_CHUNK_HEAD_OP_H_

#include <stdint.h>

namespace chef {

  struct chunk_head {
    uint64_t id_;
    uint32_t type_;
    /// 这个字段不暴露给用户，内部校验数据正确性用
    // uint32_t magic_num_;
    uint32_t reserved_;
    uint32_t body_len_;
  };

  class chunk_head_op {
    public:
      /**
       * @param ch       填充好待编码的chunk_head字段
       * @param raw_head
       *   传出参数，raw_head内存需在调用该函数前申请，并确保size >= CHUNK_HEAD_LEN
       *
       */
      static void encode(const chunk_head &ch, char *raw_head /*out*/ );

      /**
       * @param raw_head 待解码的字节流
       * @param ch       传出参数，ch内存需在调用该函数前申请
       *
       * @return 0 成功 -1 失败(magic_num错误)
       *
       */
      static int decode(const char *raw_head, chunk_head *ch /*out*/ );

    private:
      /**
       * 统一大小端
       *
       */
      static void swap_if_big_endian(char *data, int len);

    private:
      enum {
        CHUNK_HEAD_LEN = 24,
        MAGIC_NUM = 0xB6581EB,
      };
  };

} // namespace chef

#endif // _CHEF_CHUNK_HEAD_OP_H_
