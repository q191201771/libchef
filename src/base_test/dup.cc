#include "../base/chef_encoding_base64_op.hpp"
#include <map>
#include "./common/assert_wrapper.hpp"
#include "./common/check_log.hpp"

int func() {
  ENTER_TEST;

  std::map<std::string, std::string> mapping;
  mapping["\x14\xfb\x9c\x03\xd9\x7e"] = "FPucA9l+";
  mapping["\x14\xfb\x9c\x03\xd9"] = "FPucA9k=";
  mapping["\x14\xfb\x9c\x03"] = "FPucAw==";
  mapping[""] = "";
  mapping["f"] = "Zg==";
  mapping["foob"] = "Zm9vYg==";
  mapping["sure."] = "c3VyZS4=";
  mapping["leasure."] = "bGVhc3VyZS4=";
  mapping["asure."] = "YXN1cmUu";

  for (auto &iter : mapping) {
    assert(chef::encoding_base64_op::encode(iter.first) == iter.second);
    assert(chef::encoding_base64_op::decode(iter.second) == iter.first);
  }

  return 0;
}

