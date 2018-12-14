#include "chef_base/chef_consistent_hash.hpp"
#include "chef_base/chef_crypto_md5_op.hpp"
#include "./common/assert_wrapper.hpp"
#include "./common/check_log.hpp"
#include <string>
#include <vector>

static void example() {

}

int main() {
  ENTER_TEST;

  example();

  static const int NODE_NUM = 10;
  static const int VIRTUAL_MULTIS = 128;
  static const int CHOICE_NUM = 10000; // 100 * 10000;

  std::vector<std::string> nodes;
  std::vector<std::string> keys;
  chef::consistent_hash<std::string, VIRTUAL_MULTIS> ch;

  // 添加节点
  srand(time(NULL));
  for (int i = 0; i < NODE_NUM; i++) {
    std::string node = std::to_string(rand());
    //node = chef::crypto_md5_op::sum(node);
    ch.add_node(node);

    nodes.push_back(node);
  }

  // 生成所有待hash的key
  for (int i = 0; i< CHOICE_NUM; i++) {
    std::string key = std::to_string(rand());
    //key = chef::crypto_md5_op::sum(key);
    keys.push_back(key);
  }

  // 初次哈希
  std::map<std::string, int> counts;
  std::map<std::string, std::string> key2node;
  for (int i = 0; i< CHOICE_NUM; i++) {
    std::string val;
    bool ret = ch.choice_one_node(keys[i], val);
    assert(ret);
    counts[val]++;
    key2node[keys[i]] = val;
    //std::cout << key << " " << val << std::endl;
  }

  // 打印所有节点对应的哈希计数
  std::cout << "-----Init hash result-----" << std::endl;
  std::map<std::string, int>::iterator iter = counts.begin();
  for (; iter != counts.end(); ++iter) {
    std::cout << iter->first << " " << iter->second << std::endl;
  }
  std::cout << "--------------------------" << std::endl;

  // 删除节点
  ch.del_node(nodes[0]);
  //ch.del_node(nodes[1]);
  std::cout << "Del node(" << nodes[0] << ") count(" << counts[nodes[0]] << ")" << std::endl;
  //std::cout << "Del node(" << nodes[1] << ") count(" << counts[nodes[1]] << ")" << std::endl;

  // 重新哈希
  int rehit = 0;
  std::map<std::string, int> counts2;
  for (int i = 0; i< CHOICE_NUM; i++) {
    std::string val;
    bool ret = ch.choice_one_node(keys[i], val);
    assert(ret);
    if (key2node[keys[i]] == val) {
      rehit++;
    }
    counts2[val]++;
  }

  // 打印哈希结果相同的计数
  std::cout << "Rehit " << rehit << std::endl;

  // 打印所有节点哈希结果
  std::cout << "-----After del, hash result-----" << std::endl;
  std::map<std::string, int>::iterator iter2 = counts2.begin();
  for (; iter2 != counts2.end(); ++iter2) {
    std::cout << iter2->first << " " << iter2->second << std::endl;
  }
  std::cout << "--------------------------------" << std::endl;

  // // 测试一下多次添加删除
  // if (nodes.size() > 7) {
  //   ch.del_node(nodes[0]);ch.add_node(nodes[0]);
  //   ch.del_node(nodes[1]);ch.del_node(nodes[4]);ch.add_node(nodes[1]);ch.add_node(nodes[4]);
  //   ch.del_node(nodes[2]);ch.del_node(nodes[7]);
  //   ch.add_node(nodes[2]);ch.add_node(nodes[7]);
  // }

  // // 测试随机再新增一个点
  // std::string node = std::to_string(rand());
  // std::cout << "New node " << node << std::endl;
  // //node = chef::crypto_md5_op::sum(node);
  // ch.add_node(node);

  // 添加回删除掉的节点
  ch.add_node(nodes[0]);
  //ch.add_node(nodes[1]);

  // 重新哈希
  rehit = 0;
  std::map<std::string, int> counts3;
  for (int i = 0; i< CHOICE_NUM; i++) {
    std::string val;
    bool ret = ch.choice_one_node(keys[i], val);
    assert(ret);
    if (key2node[keys[i]] == val) {
      rehit++;
    }
    counts3[val]++;
  }

  // 打印和初始节点集合时哈希结果相同的计数
  std::cout << "Rehit " << rehit << std::endl;

  // 打印所有节点哈希结果
  std::cout << "-----After Add, hash result-----" << std::endl;
  std::map<std::string, int>::iterator iter3 = counts3.begin();
  for (; iter3 != counts3.end(); ++iter3) {
    std::cout << iter3->first << " " << iter3->second << std::endl;
  }
  std::cout << "--------------------------------" << std::endl;

  assert(!ch.is_node_exist(""));
  assert(ch.is_node_exist(nodes[0]));
  assert(ch.get_all_nodes().size() == NODE_NUM);
  ch.clear_all_nodes();
  assert(ch.get_all_nodes().empty());

  return 0;
}
