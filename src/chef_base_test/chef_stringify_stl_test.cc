#include "../chef_base/chef_stringify_stl.hpp"
#include <string>
#include <map>
#include <iostream>
#include "./common/assert_wrapper.hpp"
#include "./common/check_log.hpp"

struct SelfType {
  int a_;
  int b_;

  SelfType(int a, int b) : a_(a), b_(b) {  }
};

std::string stringify_stl_one_(const SelfType &st) {
  std::ostringstream oss;
  oss << "(" << st.a_ << "," << st.b_ << ")";
  return oss.str();
}

static void test_self_stype() {
  std::vector<SelfType> vec;
  vec.push_back(SelfType(1,2));
  vec.push_back(SelfType(3,4));
  std::string str = chef::stringify_stl(vec);
  assert(str == "[(1,2),(3,4)]");
  //std::cout << str << std::endl;
}

int main() {
  ENTER_TEST;

  test_self_stype();

  std::string str;

  {
    std::vector<int> vec = {1, 2, 3};
    str = chef::stringify_stl_one_(vec, chef::STRINGIFY_STL_STYLE_ONE_BEAUTY);
    (void)str;
    //std::cout << str << std::endl;

    str = chef::stringify_stl(vec);
    assert(str == "[1,2,3]");

    str = chef::stringify_stl(vec, chef::STRINGIFY_STL_STYLE_ONE_BEAUTY);
    (void)str;
    //std::cout << str << std::endl;
  }

  {
    std::vector<uint32_t> vec = {1, 2, 3};
    str = chef::stringify_stl(vec);
    assert(str == "[1,2,3]");
  }

  {
    std::vector<std::string> vec = {"1", "2", "3"};
    str = chef::stringify_stl(vec);
    assert(str == "[1,2,3]");
  }

  {
    std::vector<std::vector<int> > vec = {{1,2,3}, {4,5,6}};
    str = chef::stringify_stl(vec);
    assert(str == "[[1,2,3],[4,5,6]]");
  }

  {
    std::map<int, int> m;
    m[1] = 100; m[2] = 200; m[3] = 300;
    str = chef::stringify_stl(m, chef::STRINGIFY_STL_STYLE_TWO_BEAUTY);
    (void)str;
    //std::cout << str << std::endl;

    str = chef::stringify_stl(m);
    assert(str == "{1:100,2:200,3:300}")
  }

  {
    std::array<int, 3> arr = {{1, 2, 3}};
    str = chef::stringify_stl(arr);
    assert(str == "[1,2,3]");
  }

  {
    std::deque<int> ct = {1, 2, 3};
    str = chef::stringify_stl(ct);
    assert(str == "[1,2,3]");
  }

  {
    std::forward_list<int> ct = {1, 2, 3};
    str = chef::stringify_stl(ct);
    assert(str == "[1,2,3]");
  }

  {
    std::list<int> ct = {1, 2, 3};
    str = chef::stringify_stl(ct);
    assert(str == "[1,2,3]");
  }

  {
    std::set<int> ct = {1, 2, 3};
    str = chef::stringify_stl(ct);
    assert(str == "[1,2,3]");
  }

  {
    std::unordered_set<int> ct = {1, 2, 3};
    str = chef::stringify_stl(ct);
    assert(str == "[1,2,3]" || str == "[1,3,2]" || str == "[2,1,3]" || str == "[2,3,1]" || str == "[3,1,2]" || str == "[3,2,1]");
  }

  {
    std::multiset<int> ct = {1, 1, 2, 3};
    str = chef::stringify_stl(ct);
    assert(str == "[1,1,2,3]");
  }

  {
    std::unordered_multiset<int> ct = {1, 2, 3};
    str = chef::stringify_stl(ct);
    assert(str == "[1,2,3]" || str == "[1,3,2]" || str == "[2,1,3]" || str == "[2,3,1]" || str == "[3,1,2]" || str == "[3,2,1]");
  }

  {
    std::multimap<int, int> m;
    m.insert(std::make_pair(1, 100));
    m.insert(std::make_pair(2, 200));
    m.insert(std::make_pair(3, 300));
    str = chef::stringify_stl(m);
    assert(str == "{1:100,2:200,3:300}")
  }

  {
    std::unordered_map<int, int> m;
    m.insert(std::make_pair(1, 100));
    m.insert(std::make_pair(2, 200));
    str = chef::stringify_stl(m);
    assert(str == "{1:100,2:200}" || str == "{2:200,1:100}");
  }

  {
    std::unordered_multimap<int, int> m;
    m.insert(std::make_pair(1, 100));
    m.insert(std::make_pair(2, 200));
    str = chef::stringify_stl(m);
    assert(str == "{1:100,2:200}" || str == "{2:200,1:100}");
  }

  {
    std::stack<int> s;
    s.push(1);s.push(2);s.push(3);s.push(5);
    str = chef::stringify_stl(s);
    assert(str == "[5,3,2,1]");
    assert(s.size() == 4 && s.top() == 5);
  }

  {
    std::queue<int> q;
    q.push(1);q.push(2);q.push(3);q.push(5);q.pop();
    str = chef::stringify_stl(q);
    assert(str == "[2,3,5]");
    assert(q.size() == 3 && q.front() == 2 && q.back() == 5);
  }

  {
    std::priority_queue<int> q;
    q.push(1);q.push(2);q.push(4);q.push(5);q.pop();
    str = chef::stringify_stl(q);
    assert(str == "[4,2,1]");
    assert(q.size() == 3 && q.top() == 4);
  }

  return 0;
}

