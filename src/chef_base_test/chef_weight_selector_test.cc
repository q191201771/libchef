#include "../chef_base/chef_weight_selector.hpp"
#include "./common/assert_wrapper.hpp"
#include "./common/check_log.hpp"
#include <string>

static void example() {
  chef::weight_selector<std::string> ws;
  ws.add("a", 2);
  ws.add("b", 1);
  ws.add("c", 4);
  std::string val;
  for (int i = 0; i < 7; i++) {
    bool ret = ws.choice_one(val);
    assert(ret);
    //std::cout << val << " ";
  }
  //std::cout << std::endl;
}

int main() {
  ENTER_TEST;

  example();

  if (1) { // 循环输出
  const char seqs[] = "cacbcac";

  chef::weight_selector<std::string> ws;
  ws.add("a", 2);
  ws.add("b", 1);
  ws.add("c", 4);
  std::string val;
  for (int i = 0; i < 21; i++) {
    bool ret = ws.choice_one(val);
    assert(ret);
    assert(seqs[i % 7] == val[0]);
    //std::cout << val << " ";
  }
  //std::cout << std::endl;
  }

  { // weight值相同
  chef::weight_selector<std::string> ws;
  ws.add("a", 1);
  ws.add("b", 1);
  std::string val;
  for (int i = 0; i < 16; i++) {
    bool ret = ws.choice_one(val);
    assert(ret);
    //std::cout << val << " ";
  }
  //std::cout << std::endl;
  }

  { // 无元素或weight都为0
    chef::weight_selector<std::string> ws;
    std::string val;
    bool ret;
    ret = ws.choice_one(val);
    assert(!ret);
    ws.add("a", 0);
    ret = ws.choice_one(val);
    assert(!ret);
  }

  { // 修改weight值
  const char seqs1[] = "cacbcac";
  const char seqs2[] = "adbadad";
  const char seqs3[] = "adcbadcad";
  const int ROUND_COUNT = 3;

  chef::weight_selector<std::string> ws;
  ws.add("a", 2);
  ws.add("b", 1);
  ws.add("c", 4);
  std::string val;
  for (int i = 0; i < 7 * ROUND_COUNT; i++) {
    bool ret = ws.choice_one(val);
    assert(ret);
    //std::cout << val << " ";
    assert(seqs1[i % 7] == val[0]);
  }
  //std::cout << std::endl;

  ws.add("a", 3); // 修改
  ws.add("d", 3); // 新增
  ws.add("c", 0); // 关闭
  for (int i = 0; i < 7 * ROUND_COUNT; i++) {
    bool ret = ws.choice_one(val);
    assert(ret);
    assert(seqs2[i % 7] == val[0]);
    //std::cout << val << " ";
  }
  //std::cout << std::endl;

  ws.add("c", 2); // 重新打开
  for (int i = 0; i < 9 * ROUND_COUNT; i++) {
    bool ret = ws.choice_one(val);
    assert(ret);
    assert(seqs3[i % 9] == val[0]);
    //std::cout << val << " ";
  }
  //std::cout << std::endl;
  }

  { // 一个round没有结束时修改weight
  chef::weight_selector<std::string> ws;
  ws.add("a", 2);
  ws.add("b", 1);
  ws.add("c", 4);
  std::string val;
  for (int i = 0; i < 3; i++) {
    bool ret = ws.choice_one(val);
    assert(ret);
    //std::cout << val << " ";
  }
  //std::cout << std::endl;
  ws.add("d", 3);
  std::map<std::string, uint32_t> counts;
  for (int i = 0; i < 10001; i++) {
    // 每次choice前都add去干扰
    if (i % 2) { ws.add("e", 1); }
    else { ws.add("e", 0); }

    bool ret = ws.choice_one(val);
    assert(ret);
    counts[val]++;
  }
  //std::cout << counts["a"] << " " << counts["b"] << " " << counts["c"] << " " << counts["d"] << std::endl;
  //std::cout << counts["e"] << std::endl;
  }

  return 0;
}
