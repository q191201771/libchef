#include "../base/chef_lru.hpp"
#include "./common/assert_wrapper.hpp"
#include "./common/check_log.hpp"
#include <string>

int main() {
  ENTER_TEST;

  int v;
  chef::Lru<std::string, int>::List ll;
  chef::Lru<std::string, int>::List::iterator iter;

  chef::Lru<std::string, int> lru(3);

  // empty 空
  assert(lru.get("notexist", &v) == false);
  assert(lru.size() == 0 && lru.capacity() == 3);
  assert(lru.get_list().empty());

  // put chef 只插入一个元素
  assert(lru.put("chef", 60));
  assert(lru.size() == 1 && lru.capacity() == 3);
  ll = lru.get_list();
  assert(ll.size() == 1);
  iter = ll.begin();
  assert(iter->first == "chef" && iter->second == 60);

  // get chef 获取插入的元素
  assert(lru.get("chef", &v) && v == 60);
  assert(lru.size() == 1 && lru.capacity() == 3);
  ll = lru.get_list();
  assert(ll.size() == 1);
  iter = ll.begin();
  assert(iter->first == "chef" && iter->second == 60);

  // get ne 获取不存在的元素
  assert(lru.get("ne", &v) == false);
  assert(lru.size() == 1 && lru.capacity() == 3);
  ll = lru.get_list();
  assert(ll.size() == 1);
  iter = ll.begin();
  assert(iter->first == "chef" && iter->second == 60);

  // put yoko 插入第二个元素
  assert(lru.put("yoko", 100));
  assert(lru.size() == 2 && lru.capacity() == 3);
  ll = lru.get_list();
  assert(ll.size() == 2);
  iter = ll.begin();
  assert(iter->first == "yoko" && iter->second == 100);
  iter++;
  assert(iter->first == "chef" && iter->second == 60);

  // get yoko 获取第二个元素
  assert(lru.get("yoko", &v) && v == 100);
  assert(lru.size() == 2 && lru.capacity() == 3);
  ll = lru.get_list();
  assert(ll.size() == 2);
  iter = ll.begin();
  assert(iter->first == "yoko" && iter->second == 100);
  iter++;
  assert(iter->first == "chef" && iter->second == 60);

  // get chef 获取第一个元素，注意，此时热度会受影响
  assert(lru.get("chef", &v) && v == 60);
  assert(lru.size() == 2 && lru.capacity() == 3);
  ll = lru.get_list();
  assert(ll.size() == 2);
  iter = ll.begin();
  assert(iter->first == "chef" && iter->second == 60);
  iter++;
  assert(iter->first == "yoko" && iter->second == 100);

  // put coco 插入第三个元素
  assert(lru.put("coco", 33));
  assert(lru.size() == 3 && lru.capacity() == 3);
  ll = lru.get_list();
  assert(ll.size() == 3);
  iter = ll.begin();
  assert(iter->first == "coco" && iter->second == 33);
  iter++;
  assert(iter->first == "chef" && iter->second == 60);
  iter++;
  assert(iter->first == "yoko" && iter->second == 100);

  // put dad 插入第四个元素
  assert(lru.put("dad", 44));
  assert(lru.size() == 3 && lru.capacity() == 3);
  ll = lru.get_list();
  assert(ll.size() == 3);
  iter = ll.begin();
  assert(iter->first == "dad" && iter->second == 44);
  iter++;
  assert(iter->first == "coco" && iter->second == 33);
  iter++;
  assert(iter->first == "chef" && iter->second == 60);

  // put coco 插入已存在元素
  assert(lru.put("coco", 1000) == false);
  assert(lru.size() == 3 && lru.capacity() == 3);
  ll = lru.get_list();
  assert(ll.size() == 3);
  iter = ll.begin();
  assert(iter->first == "coco" && iter->second == 1000);
  iter++;
  assert(iter->first == "dad" && iter->second == 44);
  iter++;
  assert(iter->first == "chef" && iter->second == 60);

  // put chef 插入已存在元素，并且value值也相同
  assert(lru.put("dad", 44) == false);
  assert(lru.size() == 3 && lru.capacity() == 3);
  ll = lru.get_list();
  assert(ll.size() == 3);
  iter = ll.begin();
  assert(iter->first == "dad" && iter->second == 44);
  iter++;
  assert(iter->first == "coco" && iter->second == 1000);
  iter++;
  assert(iter->first == "chef" && iter->second == 60);

  return 0;
}

