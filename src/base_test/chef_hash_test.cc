#include "../base/chef_hash.hpp"
#include <string>
#include <map>
#include <vector>
#include <memory>
#include <cstdlib>
#include <ctime>
#include "./common/check_log.hpp"

static int id = 0;

struct student {
  public:
    student(const std::string &name, int age)
      : id_(id++)
      , name_(name)
      , age_(age)
    {}

    int         id_;
    std::string name_;
    int         age_;
};

int random_age() {
  return std::rand() % 100;
}

std::string random_name() {
  std::string ret;
  char letters[] = "abcdefghijklmnopqrstuvwxyz";
  int length = std::rand() % 26 + 1;
  for (int i = 0; i < length; i++) {
    ret += letters[std::rand() % 26];
  }
  return ret;
}

typedef std::map<size_t, int> code2count;

void add(code2count &cc, size_t code) {
  auto iter = cc.find(code);
  if (iter == cc.end()) {
    cc[code] = 1;
  } else {
    iter->second++;
  }
}

void print(const std::string &name, const code2count &cc) {
  int max_count = 0;
  size_t max_count_code = -1;
  for (auto &item : cc) {
    if (item.second > max_count) {
      max_count = item.second;
      max_count_code = item.first;
    }
  }

  printf("-----%s-----\n", name.c_str());
  printf("size: %lu\n", cc.size());
  printf("max_count: %d hash_code: %lu\n", max_count, max_count_code);
  printf("\n");
}

int main() {
  ENTER_TEST;

  std::srand(static_cast<unsigned int>(std::time(0)));

  const int NUM = 1000 * 1000;

  std::vector<std::shared_ptr<student> > students;
  for (int i = 0; i < NUM; i++) {
    students.push_back(std::make_shared<student>(random_name(), random_age()));
  }

  code2count id_cc;
  code2count name_cc;
  code2count age_cc;
  code2count student_cc;

  for (auto &s : students) {
    size_t id_hash_code = std::hash<int>()(s->id_) % NUM;
    size_t name_hash_code = std::hash<std::string>()(s->name_) % NUM;
    size_t age_hash_code = std::hash<int>()(s->age_) % NUM;
    size_t student_hash_code = chef::hash_val(s->id_, s->name_, s->age_) % NUM;
    add(id_cc, id_hash_code);
    add(name_cc, name_hash_code);
    add(age_cc, age_hash_code);
    add(student_cc, student_hash_code);
  }

  print("id", id_cc);
  print("name", name_cc);
  print("age", age_cc);
  print("student", student_cc);

  return 0;
}
