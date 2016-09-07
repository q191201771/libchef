#include "chunk_head_op.h"
#include <string.h>

namespace chef {

  void chunk_head_op::swap_if_big_endian(char *data, int len) {
    union {
      char c;
      int i;
    }n;
    n.i = 1;
    if (n.c != 1) {
      for (int i = 0; i < len / 2; ++i) {
        char tmp = data[i];
        data[i] = data[len - 1 - i];
        data[len - 1 - i] = tmp;
      }
    }
  }

  void chunk_head_op::encode(const chunk_head &ch, char *raw_head) {
    uint64_t id = ch.id_;
    swap_if_big_endian(reinterpret_cast<char*>(&id), sizeof id);
    memcpy(raw_head, &id, sizeof id);
    uint32_t type = ch.type_;
    swap_if_big_endian(reinterpret_cast<char*>(&type), sizeof type);
    memcpy(raw_head + 8, &type, sizeof type);
    uint32_t magic_num = MAGIC_NUM;
    swap_if_big_endian(reinterpret_cast<char*>(&magic_num), sizeof magic_num);
    memcpy(raw_head + 12, &magic_num, sizeof magic_num);
    uint32_t reserved = ch.reserved_;
    swap_if_big_endian(reinterpret_cast<char*>(&reserved), sizeof reserved);
    memcpy(raw_head + 16, &reserved, sizeof reserved);
    uint32_t body_len = ch.body_len_;
    swap_if_big_endian(reinterpret_cast<char*>(&body_len), sizeof body_len);
    memcpy(raw_head + 20, &body_len, sizeof body_len);
  }

  int chunk_head_op::decode(const char *raw_head, chunk_head *ch) {
    uint32_t magic_num;
    memcpy(reinterpret_cast<void *>(&magic_num), raw_head + 12, sizeof magic_num);
    swap_if_big_endian(reinterpret_cast<char*>(&magic_num), sizeof magic_num);
    if (magic_num != MAGIC_NUM) {
      return -1;
    }

    memcpy(&ch->id_, raw_head, sizeof ch->id_);
    swap_if_big_endian(reinterpret_cast<char*>(&ch->id_), sizeof ch->id_);
    memcpy(&ch->type_, raw_head + 8, sizeof ch->type_);
    swap_if_big_endian(reinterpret_cast<char*>(&ch->type_), sizeof ch->type_);
    memcpy(&ch->reserved_, raw_head + 16, sizeof ch->reserved_);
    swap_if_big_endian(reinterpret_cast<char*>(&ch->reserved_), sizeof ch->reserved_);
    memcpy(&ch->body_len_, raw_head + 20, sizeof ch->body_len_);
    swap_if_big_endian(reinterpret_cast<char*>(&ch->body_len_), sizeof ch->body_len_);

    return 0;
  }

} // namespace chef
