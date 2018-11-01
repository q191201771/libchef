#include "../chef_base/chef_lru.hpp"
#include "./common/assert_wrapper.hpp"
#include "./common/check_log.hpp"
#include <string>

static void example() {
     chef::lru<std::string, int> c(3);
     c.put("chef", 1);
     c.put("yoko", 2);
     c.put("tom", 3);
     c.put("jerry", 4); // 超过容器大小，淘汰最老的`chef`
     bool exist;
     int v;
     exist = c.get("chef", &v);
     assert(!exist);
     exist = c.get("yoko", &v);
     assert(exist && v == 2);
     c.put("garfield", 5); // 超过容器大小，注意，由于`yoko`刚才读取时会更新热度，所以淘汰的是`tom`
     exist = c.get("yoko", &v);
     assert(exist && v == 2);
     exist = c.get("tom", &v);
     assert(!exist);
}

int main() {
  ENTER_TEST;

  example();

  int v;
  chef::lru<std::string, int>::List ll;
  chef::lru<std::string, int>::List::iterator iter;

  chef::lru<std::string, int> lru(3);

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
  ++iter;
  assert(iter->first == "chef" && iter->second == 60);

  // get yoko 获取第二个元素
  assert(lru.get("yoko", &v) && v == 100);
  assert(lru.size() == 2 && lru.capacity() == 3);
  ll = lru.get_list();
  assert(ll.size() == 2);
  iter = ll.begin();
  assert(iter->first == "yoko" && iter->second == 100);
  ++iter;
  assert(iter->first == "chef" && iter->second == 60);

  // get chef 获取第一个元素，注意，此时热度会受影响
  assert(lru.get("chef", &v) && v == 60);
  assert(lru.size() == 2 && lru.capacity() == 3);
  ll = lru.get_list();
  assert(ll.size() == 2);
  iter = ll.begin();
  assert(iter->first == "chef" && iter->second == 60);
  ++iter;
  assert(iter->first == "yoko" && iter->second == 100);

  // put coco 插入第三个元素
  assert(lru.put("coco", 33));
  assert(lru.size() == 3 && lru.capacity() == 3);
  ll = lru.get_list();
  assert(ll.size() == 3);
  iter = ll.begin();
  assert(iter->first == "coco" && iter->second == 33);
  ++iter;
  assert(iter->first == "chef" && iter->second == 60);
  ++iter;
  assert(iter->first == "yoko" && iter->second == 100);

  // put dad 插入第四个元素
  assert(lru.put("dad", 44));
  assert(lru.size() == 3 && lru.capacity() == 3);
  ll = lru.get_list();
  assert(ll.size() == 3);
  iter = ll.begin();
  assert(iter->first == "dad" && iter->second == 44);
  ++iter;
  assert(iter->first == "coco" && iter->second == 33);
  ++iter;
  assert(iter->first == "chef" && iter->second == 60);

  // put coco 插入已存在元素
  assert(lru.put("coco", 1000) == false);
  assert(lru.size() == 3 && lru.capacity() == 3);
  ll = lru.get_list();
  assert(ll.size() == 3);
  iter = ll.begin();
  assert(iter->first == "coco" && iter->second == 1000);
  ++iter;
  assert(iter->first == "dad" && iter->second == 44);
  ++iter;
  assert(iter->first == "chef" && iter->second == 60);

  // put chef 插入已存在元素，并且value值也相同
  assert(lru.put("dad", 44) == false);
  assert(lru.size() == 3 && lru.capacity() == 3);
  ll = lru.get_list();
  assert(ll.size() == 3);
  iter = ll.begin();
  assert(iter->first == "dad" && iter->second == 44);
  ++iter;
  assert(iter->first == "coco" && iter->second == 1000);
  ++iter;
  assert(iter->first == "chef" && iter->second == 60);

  return 0;
}

