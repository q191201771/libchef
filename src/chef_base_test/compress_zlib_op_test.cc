/**
 * @file   compress_zlib_op.cc
 * @author
 *   chef <191201771@qq.com>
 *     -created 2017-06-03 10:28:06
 *     -initial release xxxx-xx-xx
 *
 */

#include "../base/compress_zlib_op.h"
#include "./common/assert_wrapper.hpp"
#include "./common/check_log.hpp"
#include <string.h>
#include <vector>
#include <string>

namespace ut {

  void test(const unsigned char *raw_data, int raw_data_len, chef::compress_zlib_op::compression_level level) {
    unsigned char *compressed_data = NULL;
    int compressed_data_len;
    int ret = chef::compress_zlib_op::compress(raw_data, raw_data_len, &compressed_data, &compressed_data_len, level);
    assert(ret == 0);
    //printf("%d -> %d\n", raw_data_len, compressed_data_len);
    //for (int i = 0; i < compressed_data_len; i++) {
    //  printf("%d ", compressed_data[i]);
    //}
    //printf("\n");
    unsigned char *decompressed_data = NULL;
    int decompressed_data_len;
    ret = chef::compress_zlib_op::decompress(compressed_data, compressed_data_len, &decompressed_data, &decompressed_data_len);
    assert(ret == 0 && decompressed_data_len == raw_data_len && memcmp(raw_data, decompressed_data, raw_data_len) == 0);
    free(compressed_data);
    free(decompressed_data);
  }

}


int main() {
  ENTER_TEST;

  int raw_data_len;
  unsigned char *raw_data;

  std::vector<int> raw_data_len_vec;
  int LOOP_NUM = 20; //30;
  for (int i = 1; i < LOOP_NUM; i++) {
    raw_data_len_vec.push_back((1 << i) - 1);
    raw_data_len_vec.push_back(1 << i);
    raw_data_len_vec.push_back((1 << i) + 1);
  }

  std::vector<int>::iterator iter = raw_data_len_vec.begin();
  for (; iter != raw_data_len_vec.end(); iter++) {
    raw_data_len = *iter;
    raw_data = static_cast<unsigned char *>(calloc(raw_data_len, 1));
    ut::test(raw_data, raw_data_len, chef::compress_zlib_op::COMPRESSION_LEVEL_DEFAULT);
    ut::test(raw_data, raw_data_len, chef::compress_zlib_op::COMPRESSION_LEVEL_BEST_SPEED);
    ut::test(raw_data, raw_data_len, chef::compress_zlib_op::COMPRESSION_LEVEL_BEST_COMPRESSION);
    free(raw_data);
  }

  const char *CASE = "hello world!";
  ut::test(reinterpret_cast<const unsigned char *>(CASE), static_cast<int>(strlen(CASE)), chef::compress_zlib_op::COMPRESSION_LEVEL_DEFAULT);

  return 0;
}
