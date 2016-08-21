#include "../base/encoding_base64_op.hpp"
#include <map>
#include <assert.h>

int main() {
  printf("Check encoding_base64.\n");
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

  auto iter = mapping.begin();
  for (; iter != mapping.end(); iter++) {
    assert(chef::base64_encode(iter->first) == iter->second);
    assert(chef::base64_decode(iter->second) == iter->first);
  }

  printf("Check encoding_base64 done.\n");
  return 0;
}
