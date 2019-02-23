#include "chef_base/chef_be_le_op.hpp"
#include "./common/assert_wrapper.hpp"
#include "./common/check_log.hpp"
#include <arpa/inet.h>

int main() {
  ENTER_TEST;

  assert(chef::be_le_op::check_local_if_le());
  uint32_t val = 0x12345678;
  uint32_t be_val = htonl(val);
  assert(be_val = 0x78563412);

  uint8_t *p = new uint8_t[128];
  chef::be_le_op::write_be_ui8(p, 10);
  assert(chef::be_le_op::read_be_ui8(p) == 10);
  chef::be_le_op::write_be_ui16(p, 0x1122);
  assert(chef::be_le_op::read_be_ui16(p) == 0x1122);
  chef::be_le_op::write_be_ui24(p, 0x112233);
  assert(chef::be_le_op::read_be_ui24(p) == 0x112233);
  chef::be_le_op::write_be_ui32(p, val);
  assert(*reinterpret_cast<uint32_t *>(p) == be_val);
  assert(chef::be_le_op::read_be_ui32(p) == val);

  chef::be_le_op::write_le_ui32(p, val);
  assert(*reinterpret_cast<uint32_t *>(p) == val);
  assert(chef::be_le_op::read_le_ui32(p) == val);

  delete []p;

  return 0;
}
