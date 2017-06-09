/**
 * @file   crypto_sha1_op.h/crypto_sha1_op.cc
 * @deps   xxx
 *
 * @author
 *   chef <191201771@qq.com>
 *     -created 2017-06-09 09:28:48
 *     -initial release xxxx-xx-xx
 *
 * @brief
 *   sha1算法
 *   NOTICE: 实现部分拷贝自redis3.2.9中的sha1.h和sha1.c https://github.com/antirez/redis/releases/tag/3.2.9
 *
 */

#pragma once

#include <string>

namespace chef {

class crypto_sha1_op {
  public:
    static void sum(unsigned char *in_data, size_t in_data_len, unsigned char digest[20] /*out*/);

    /**
     * @return 对`data`执行sha1编码后得到的20字节，每个字节用16进制表示（2字节）拼接成的字符串
     *
     */
    static std::string sum(const std::string &data);
}; /// class crypto_sha1_op

} /// namespace chef

