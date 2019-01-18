#include "chef_base/chef_skiplist.hpp"
#include "chef_base/chef_this_process_op.hpp"
#include "./common/assert_wrapper.hpp"
#include "./common/check_log.hpp"
#include <map>
#include <string>
#include <vector>
#include <time.h>
#include <sys/time.h>

static uint64_t tickkk() {
    struct timespec ts;
#if defined(CLOCK_REALTIME) && !defined(__MACH__)
    clock_gettime(CLOCK_MONOTONIC, &ts);
#else
    {
      struct timeval tv;
      gettimeofday(&tv, NULL);
      ts.tv_sec = tv.tv_sec;
      ts.tv_nsec = tv.tv_usec * 1000;
    }
#endif
    return ts.tv_sec * 1000 + ts.tv_nsec / 1000000;
  }

static constexpr int NUM = 10 * 10000;
std::vector<int> vec;
std::vector<int> vec2;
std::map<int, int> gm;
chef::skiplist<int, int> gsl;

static void reset() {
  gm.clear();
  for (int i = 0; i < NUM; ++i) {
    gm.insert(std::make_pair(vec[i], i));
  }

  gsl.clear();
  for (int i = 0; i < NUM; ++i) {
    gsl.insert(std::make_pair(vec[i], i));
  }
}

static void benchmark() {
  std::mt19937 rg(std::chrono::system_clock::now().time_since_epoch().count());
  uint64_t t;
  uint64_t md;
  uint64_t sld;
  bool bres;
  chef::this_process_op::mem_info mi;
  chef::this_process_op::mem_info mi2;

  for (int i = 0; i < NUM; ++i) {
    vec.push_back(rg() % NUM);
  }

  for (int i = 0; i < NUM; ++i) {
    vec2.push_back(rg() % (NUM));
  }

  /// 插入
  //bres = chef::this_process_op::obtain_mem_info(&mi);
  //assert(bres);
  t = tickkk();
  for (int i = 0; i < NUM; ++i) {
    gm.insert(std::make_pair(vec[i], i));
  }
  md = tickkk() - t;
  //bres = chef::this_process_op::obtain_mem_info(&mi2);
  //assert(bres);
  std::cout << "map res_kbytes diff:" << mi2.res_kbytes - mi.res_kbytes << std::endl;

  //bres = chef::this_process_op::obtain_mem_info(&mi);
  //assert(bres);
  t = tickkk();
  for (int i = 0; i < NUM; ++i) {
    gsl.insert(std::make_pair(vec[i], i));
  }
  sld = tickkk() - t;
  //bres = chef::this_process_op::obtain_mem_info(&mi2);
  //assert(bres);
  std::cout << "skiplist res_kbytes diff:" << mi2.res_kbytes - mi.res_kbytes << std::endl;

  std::cout << "orig vector size:" << vec.size() << " map size:" << gm.size() << " skiplist size:" << gsl.size() << std::endl;
  std::cout << "Insert cost:" << md << " " << sld << std::endl;

  /// 范围构造
  {}

  /// 拷贝构造
  {
  t = tickkk();
  std::map<int, int> m2(gm);
  md = tickkk() - t;

  t = tickkk();
  chef::skiplist<int, int> sl2(gsl);
  sld = tickkk() - t;
  std::cout << "Ctor copy cost:" << md << " " << sld << std::endl;
  }

  /// 赋值
  {}

  /// 遍历
  {
  int64_t mc = 0;
  int64_t slc = 0;
  t = tickkk();
  auto iter = gm.begin();
  for (; iter != gm.end(); ++iter) {
    mc += iter->first;
  }
  md = tickkk() - t;

  t = tickkk();
  auto iter2 = gsl.begin();
  for (; iter2 != gsl.end(); ++iter2) {
    slc += iter2->first;
  }
  sld = tickkk() - t;

  assert(mc == slc);
  std::cout << "Iterate cost:" << md << " " << sld << std::endl;
  }

  /// 反序遍历
  {
  int64_t mc = 0;
  int64_t slc = 0;
  t = tickkk();
  auto iter = gm.rbegin();
  for (; iter != gm.rend(); ++iter) {
    mc += iter->first;
  }
  md = tickkk() - t;

  t = tickkk();
  auto iter2 = gsl.rbegin();
  for (; iter2 != gsl.rend(); ++iter2) {
    slc += iter2->first;
  }
  sld = tickkk() - t;

  assert(mc == slc);
  std::cout << "Reserve iterate cost:" << md << " " << sld << std::endl;
  }

  /// erase
  {
    int64_t mc = 0;
    int64_t slc = 0;
    t = tickkk();
    for (auto iter = vec2.begin(); iter != vec2.end(); ++iter) {
      mc += gm.erase(*iter);
    }
    md = tickkk() - t;

    t = tickkk();
    for (auto iter = vec2.begin(); iter != vec2.end(); ++iter) {
      slc += gsl.erase(*iter);
    }
    sld = tickkk() - t;

    std::cout << "truely erase:" << mc << std::endl;
    assert(mc == slc);
    std::cout << "Erase cost:" << md << " " << sld << std::endl;
  }
  reset();

  /// operator[]
  {
    int64_t mc = 0;
    int64_t slc = 0;
    t = tickkk();
    for (auto iter = vec2.begin(); iter != vec2.end(); ++iter) {
      mc += gm[*iter];
    }
    md = tickkk() - t;

    t = tickkk();
    for (auto iter = vec2.begin(); iter != vec2.end(); ++iter) {
      slc += gsl[*iter];
    }
    sld = tickkk() - t;

    assert(mc == slc);
    std::cout << "Operator [] cost:" << md << " " << sld << std::endl;
    std::cout << "grow up map size:" << gm.size() << " " << "skiplist size:" << gsl.size() << std::endl;
  }
  reset();

  /// at
  {}

  /// insert with hint && insert range
  {}

  /// erase range
  {}

  /// swap
  {
    t = tickkk();
    std::map<int, int> m2;
    m2.swap(gm);
    gm.clear();
    m2.clear();
    md = tickkk() - t;

    t = tickkk();
    chef::skiplist<int, int> sl2;
    sl2.swap(gsl);
    gsl.clear();
    sl2.clear();
    sld = tickkk() - t;

    std::cout << "Swap and Clear cost:" << md << " " << sld << std::endl;
  }
  reset();

  /// find
  {
    int64_t mc = 0;
    int64_t slc = 0;
    t = tickkk();
    for (auto iter = vec2.begin(); iter != vec2.end(); ++iter) {
      if (gm.find(*iter) != gm.end()) { ++mc; }
    }
    md = tickkk() - t;

    t = tickkk();
    for (auto iter = vec2.begin(); iter != vec2.end(); ++iter) {
      if (gsl.find(*iter) != gsl.end()) { ++slc; }
    }
    sld = tickkk() - t;

    std::cout << "find hit:" << mc << std::endl;
    assert(mc == slc);
    std::cout << "Find cost:" << md << " " << sld << std::endl;
  }

  /// count

  /// lower_bound

  /// upper_bound

  /// equal_range
  {
    int64_t mc = 0;
    int64_t slc = 0;
    t = tickkk();
    for (auto iter = vec2.begin(); iter != vec2.end(); ++iter) {
      if (gm.equal_range(*iter).second != gm.end()) { ++mc; }
    }
    md = tickkk() - t;

    t = tickkk();
    for (auto iter = vec2.begin(); iter != vec2.end(); ++iter) {
      if (gsl.equal_range(*iter).second != gsl.end()) { ++slc; }
    }
    sld = tickkk() - t;

    assert(mc == slc);
    std::cout << "Equal range cost:" << md << " " << sld << std::endl;

  }
}

static void example() {
}

int main() {
  ENTER_TEST;

  example();
  benchmark();
  //return 0;

  {
  chef::skiplist<int, int>::iterator iter;
  chef::skiplist<int, int>::reverse_iterator riter;
  //chef::skiplist<int, int>::const_iterator citer;
  //chef::skiplist<int, int>::const_reverse_iterator criter;

  std::pair<chef::skiplist<int, int>::iterator, bool> ir;

  chef::skiplist<int, int> sl;

  // 测试迭代器参数构造
  sl.clear(); sl[1] = 10; sl[2] = 20; sl[3] = 30;
  iter = sl.find(3);
  chef::skiplist<int, int> sl2(sl.begin(), iter);
  assert(sl2.size() == 2 && sl2[1] == 10 && sl2[2] == 20);

  // 测试拷贝构造
  chef::skiplist<int, int> sl3(sl);
  assert(sl3.size() == 3 && sl3[1] == 10 && sl3[2] == 20 && sl3[3] == 30);

  // 测试赋值
  chef::skiplist<int, int> sl4;
  sl4[4] = 40;
  sl4 = sl;
  assert(sl4.size() == 3 && sl4[1] == 10 && sl4[2] == 20 && sl4[3] == 30);

  // 测试打印
  std::cout << "Testing print...\n";
  sl.clear(); sl[1] = 10; sl[2] = 20; sl[3] = 30;
  sl.debug_print();

  // 测试正序迭代遍历
  std::cout << "Testing iterate...\n";
  sl.clear(); sl[1] = 10; sl[2] = 20; sl[3] = 30;
  for (iter = sl.begin(); iter != sl.end(); ++iter) {
    ++iter->second;
    std::cout << iter->first << " " << iter->second << std::endl;
  }
  //for (citer = sl.cbegin(); citer != sl.cend(); citer++) {
  //  //citer->second++;
  //  std::cout << citer->first << " " << citer->second << std::endl;
  //}
  // 测试反序迭代遍历
  std::cout << "Testing reverse iterate...\n";
  for (riter = sl.rbegin(); riter != sl.rend(); ++riter) {
    std::cout << riter->first << " " << riter->second << std::endl;
  }
  //for (criter = sl.crbegin(); criter != sl.crend(); criter++) {
  //  std::cout << criter->first << " " << criter->second << std::endl;
  //}

  // Capacity
  sl.clear();
  assert(sl.empty());
  assert(sl.size() == 0);
  assert(sl.max_size() > 0);
  sl.clear(); sl[1] = 10; sl[2] = 20; sl[3] = 30;
  assert(!sl.empty());
  assert(sl.size() == 3);
  assert(sl.max_size() > 0);

  // 测试下标访问
  sl.clear(); sl[1] = 10; sl[2] = 20; sl[3] = 30;
  sl[1] = 1000;
  sl[4] = 400;
  assert(sl.size() == 4 && sl[1] == 1000 && sl[2] == 20 && sl[3] == 30 && sl[4] == 400);

  // 测试at访问
  sl.clear(); sl[1] = 10; sl[2] = 20; sl[3] = 30;
  assert(sl.at(1) == 10 && sl.at(2) == 20);
  try {
    sl.at(191201771);
  } catch (const std::out_of_range &oor) {
    std::cout << "Catched out of range exception: " << oor.what() << std::endl;
  }
  {
    const chef::skiplist<int, int> csl(sl.begin(), sl.end());
    assert(csl.at(1) == 10);
    //const不可修改，下面这句应该报编译错误
    //csl.at(1) = 100;
  }

  // 为空时查找
  sl.clear();
  assert(sl.find(1) == sl.end());

  // 插入首个元素
  ir = sl.insert(std::make_pair(1, 10));
  assert(ir.second);
  // 查找成功
  iter = sl.find(1);
  assert(iter->first == 1);
  // 查找失败
  iter = sl.find(2);
  assert(iter == sl.end());

  // 插入第二个元素
  ir = sl.insert(std::make_pair(2, 20));
  assert(ir.second);
  iter = sl.find(2);
  assert(iter->first == 2);

  // 插入已存在元素
  ir = sl.insert(std::make_pair(1, 30));
  assert(!ir.second);
  // 通过迭代器修改mapped value
  ir.first->second = 100;
  iter = sl.find(1);
  assert(iter->first == 1 && iter->second == 100);

  ir = sl.insert(std::make_pair(3, 30));
  assert(ir.second);

  // 提示位置插入
  sl.clear(); sl[1] = 10; sl[2] = 20; sl[3] = 30;
  iter = sl.insert(sl.begin(), std::make_pair(1, 100));
  assert(iter->first == 1 && iter->second == 10);
  iter = sl.insert(iter, std::make_pair(4, 400));
  assert(iter->first == 4 && iter->second == 400);
  iter = sl.insert(sl.end(), std::make_pair(5, 500));
  assert(iter->first == 5 && iter->second == 500);
  // 迭代器插入
  sl.clear(); sl[1] = 10; sl[2] = 20; sl[3] = 30;
  chef::skiplist<int, int> sl5;
  sl5[1] = 100;
  sl5[4] = 400;
  sl5.insert(sl.begin(), sl.end());
  assert(sl5.size() == 4 && sl5[1] == 100 && sl5[2] == 20 && sl5[3] == 30 && sl5[4] == 400);

  // 测试单个迭代器删除
  sl.clear(); sl[1] = 10; sl[2] = 20; sl[3] = 30;
  sl.erase(sl.find(1));
  assert(sl.size() == 2 && sl[2] == 20 && sl[3] == 30);
  // 测试key删除
  sl.clear(); sl[1] = 10; sl[2] = 20; sl[3] = 30;
  assert(sl.erase(3) == 1);
  assert(sl.erase(1) == 1);
  assert(sl.erase(4) == 0);
  assert(sl.size() == 1 && sl[2] == 20);
  // 测试范围迭代器key删除
  sl.clear(); sl[1] = 10; sl[2] = 20; sl[3] = 30;
  sl.erase(sl.begin(), sl.find(3));
  assert(sl.size() == 1 && sl[3] == 30);
  sl.clear(); sl[1] = 10; sl[2] = 20; sl[3] = 30;
  sl.erase(sl.begin(), sl.end());
  assert(sl.empty());

  // 测试swap
  sl.clear(); sl[1] = 10; sl[2] = 20; sl[3] = 30;
  sl2.clear(); sl2[1] = 100; sl2[2] = 200; sl2[4] = 400;
  sl.swap(sl2);
  assert(sl.size() == 3 && sl[1] == 100 && sl[2] == 200 && sl[4] == 400);
  assert(sl2.size() == 3 && sl2[1] == 10 && sl2[2] == 20 && sl2[3] == 30);

  // 测试清空
  sl.clear();
  sl.clear();
  assert(sl.size() == 0);

  // 测试count
  sl.clear(); sl[1] = 10; sl[2] = 20; sl[3] = 30;
  assert(sl.count(1) == 1 && sl.count(4) == 0);

  // 测试lower_bound
  sl.clear(); sl[10] = 10; sl[20] = 20; sl[30] = 30;
  assert(sl.lower_bound(0)->first == 10);
  assert(sl.lower_bound(10)->first == 10);
  assert(sl.lower_bound(15)->first == 20);
  assert(sl.lower_bound(20)->first == 20);
  assert(sl.lower_bound(25)->first == 30);
  assert(sl.lower_bound(30)->first == 30);
  assert(sl.lower_bound(35) == sl.end());

  // 测试upper_bound
  sl.clear(); sl[10] = 10; sl[20] = 20; sl[30] = 30;
  assert(sl.upper_bound(0)->first == 10);
  assert(sl.upper_bound(10)->first == 20);
  assert(sl.upper_bound(15)->first == 20);
  assert(sl.upper_bound(20)->first == 30);
  assert(sl.upper_bound(25)->first == 30);
  assert(sl.upper_bound(30) == sl.end());
  assert(sl.upper_bound(35) == sl.end());

  // 测试equal_range
  sl.clear(); sl[10] = 10; sl[20] = 20; sl[30] = 30;
  assert(sl.equal_range(0).first->first  == 10 && sl.equal_range(0).second->first  == 10);
  assert(sl.equal_range(10).first->first == 10 && sl.equal_range(10).second->first == 20);
  assert(sl.equal_range(15).first->first == 20 && sl.equal_range(15).second->first == 20);
  assert(sl.equal_range(20).first->first == 20 && sl.equal_range(20).second->first == 30);
  assert(sl.equal_range(25).first->first == 30 && sl.equal_range(25).second->first == 30);
  assert(sl.equal_range(30).first->first == 30 && sl.equal_range(30).second == sl.end());
  assert(sl.equal_range(35).first == sl.end() && sl.equal_range(35).second == sl.end());

  chef::skiplist<float, float> slff;
  (void)slff;
  }

  return 0;
}
