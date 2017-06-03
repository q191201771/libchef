#include "compress_zlib_op.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

#include <zlib.h>

static const int CHUNK_LEN = 8192;

namespace chef {

  int compress_zlib_op::compress(const unsigned char *in, int in_len, unsigned char **out, int *out_len,
                                 compression_level level)
  {
    if (in == NULL || in_len <= 0 || out_len == NULL) {
      return -1;
    }

    int ret, flush;
    int have;
    z_stream strm;
    unsigned char fixed_chunk[CHUNK_LEN];

    strm.zalloc = Z_NULL;
    strm.zfree  = Z_NULL;
    strm.opaque = Z_NULL;
    ret = deflateInit(&strm, level);
    if (ret != Z_OK) {
      return -1;
    }

    int out_data_capacity = in_len;
    unsigned char *out_data = static_cast<unsigned char *>(malloc(out_data_capacity));

    int left         = in_len;
    int used         = 0;
    int out_data_len = 0;

    do {
      strm.avail_in = left > CHUNK_LEN ? CHUNK_LEN : left;
      flush = left <= CHUNK_LEN ? Z_FINISH : Z_NO_FLUSH;
      strm.next_in = const_cast<unsigned char *>(in) + used;
      used += strm.avail_in;
      left -= strm.avail_in;

      do {
        strm.avail_out = CHUNK_LEN;
        strm.next_out  = fixed_chunk;
        ret = deflate(&strm, flush);
        if (ret == Z_STREAM_ERROR) {
          goto cleanup;
        }
        have = CHUNK_LEN - strm.avail_out;

        if (out_data_len + have > out_data_capacity) {
          out_data_capacity = out_data_len + have;
          out_data_capacity <<= 1;
          unsigned char *tmp = static_cast<unsigned char *>(realloc(out_data, out_data_capacity));
          if (tmp == NULL) {
            goto cleanup;
          }
          out_data = tmp;
        }

        memcpy(out_data+out_data_len, fixed_chunk, have);
        out_data_len += have;
      } while (strm.avail_out == 0);
    } while (flush != Z_FINISH);

    if (ret != Z_STREAM_END) {
      goto cleanup;
    }

    *out     = out_data;
    *out_len = out_data_len;

    (void)deflateEnd(&strm);
    return 0;

  cleanup:
    (void)deflateEnd(&strm);
    if (out_data != NULL) {
      free(out_data);
    }
    return -1;
  }

  int compress_zlib_op::decompress(const unsigned char *in, int in_len, unsigned char **out, int *out_len) {
    if (in == NULL || in_len <= 0 || out_len == NULL) {
      return -1;
    }

    int ret;
    int have;
    z_stream strm;

    unsigned char fixed_chunk[CHUNK_LEN];

    strm.zalloc   = Z_NULL;
    strm.zfree    = Z_NULL;
    strm.opaque   = Z_NULL;
    strm.avail_in = 0;
    strm.next_in  = Z_NULL;
    ret = inflateInit(&strm);
    if (ret != Z_OK) {
      return -1;
    }

    int out_data_capacity = in_len << 2;
    unsigned char *out_data = static_cast<unsigned char *>(malloc(out_data_capacity));

    int left = in_len;
    int used = 0;
    int out_data_len = 0;

    do {
      strm.avail_in = left > CHUNK_LEN ? CHUNK_LEN : left;
      if (strm.avail_in <= 0) {
        break;
      }

      strm.next_in = const_cast<unsigned char *>(in) + used;
      used += strm.avail_in;
      left -= strm.avail_in;

      do {
        strm.avail_out = CHUNK_LEN;
        strm.next_out  = fixed_chunk;
        ret = inflate(&strm, Z_NO_FLUSH);
        //assert(ret != Z_STREAM_ERROR);
        switch (ret) {
          case Z_STREAM_ERROR:
          case Z_NEED_DICT:
          case Z_DATA_ERROR:
          case Z_MEM_ERROR:
            goto cleanup;
        }
        have = CHUNK_LEN - strm.avail_out;

        if (out_data_len + have > out_data_capacity) {
          out_data_capacity = out_data_len + have;
          out_data_capacity <<= 1;
          unsigned char *tmp = static_cast<unsigned char *>(realloc(out_data, out_data_capacity));
          if (tmp == NULL) {
            goto cleanup;
          }

          out_data = tmp;
        }

        memcpy(out_data+out_data_len, fixed_chunk, have);
        out_data_len += have;
      } while (strm.avail_out == 0);
    } while (ret != Z_STREAM_END);

    if (ret != Z_STREAM_END) {
      goto cleanup;
    }

    *out_len = out_data_len;
    *out = out_data;

    (void)inflateEnd(&strm);
    return 0;

  cleanup:
      (void)inflateEnd(&strm);
      if (out_data) {
          free(out_data);
      }
      return -1;
  }

} // namespace chef

