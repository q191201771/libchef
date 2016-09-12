#include "../base/strings.h"
#include <stdio.h>
#include <string>
#include <map>
#include <vector>
#include "./common/assert_wrapper.hpp"
#include "./common/check_log.hpp"

const std::string dots = "1....2....3....4";

struct string_pair_2_int {
  std::vector<std::string> pair_;
  int result_;
  string_pair_2_int(const std::string &s1, const std::string &s2, int result) {
    pair_.push_back(s1);
    pair_.push_back(s2);
    result_ = result;
  }
};

struct string_pair_2_bool {
  std::vector<std::string> pair_;
  bool result_;
  string_pair_2_bool(const std::string &s1, const std::string &s2, bool result) {
    pair_.push_back(s1);
    pair_.push_back(s2);
    result_ = result;
  }
};

void strings_compare_test() {
  /// @NOTICE
  /// std::string::compare返回值为 0 || > 0 || < 0，而不是 0 || 1 || -1
  /// 所以比较的时候转换下
  const int POSITIVE_FLAG = 1;
  const int NEGITIVE_FLAG = -1;
  std::vector<string_pair_2_int> vec;
  vec.push_back(string_pair_2_int("", "", 0));
  vec.push_back(string_pair_2_int("abc", "abc", 0));
  vec.push_back(string_pair_2_int("abcdefgh", "abcdefgh", 0));
  vec.push_back(string_pair_2_int("abcdefghi", "abcdefghi", 0));
  vec.push_back(string_pair_2_int("a", "", POSITIVE_FLAG));
  vec.push_back(string_pair_2_int("abc", "ab", POSITIVE_FLAG));
  vec.push_back(string_pair_2_int("x", "ab", POSITIVE_FLAG));
  vec.push_back(string_pair_2_int("x", "a", POSITIVE_FLAG));
  vec.push_back(string_pair_2_int("", "a", NEGITIVE_FLAG));
  vec.push_back(string_pair_2_int("ab", "x", NEGITIVE_FLAG));
  vec.push_back(string_pair_2_int("ab", "abc", NEGITIVE_FLAG));
  vec.push_back(string_pair_2_int("b", "x", NEGITIVE_FLAG));
  vec.push_back(string_pair_2_int("abcdefghi", "abcdefghj", NEGITIVE_FLAG));

  for (auto &item : vec) {
    switch (item.result_) {
    case 0:
      assert(chef::strings::compare(item.pair_[0], item.pair_[1]) == 0);
      break;
    case POSITIVE_FLAG:
      assert(chef::strings::compare(item.pair_[0], item.pair_[1]) > 0);
      break;
    case NEGITIVE_FLAG:
      assert(chef::strings::compare(item.pair_[0], item.pair_[1]) < 0);
      break;
    }
  }
}

void strings_contains_test() {
  std::vector<string_pair_2_bool> vec;
  vec.push_back(string_pair_2_bool("abc", "bc", true));
  vec.push_back(string_pair_2_bool("abc", "bcd", false));
  vec.push_back(string_pair_2_bool("abc", "", true));
  vec.push_back(string_pair_2_bool("", "a", false));
  for (auto &item : vec) {
    assert(chef::strings::contains(item.pair_[0], item.pair_[1]) == item.result_);
  }
}

void strings_contains_any_test() {
  std::vector<string_pair_2_bool> vec;
  vec.push_back(string_pair_2_bool("", "", false));
  vec.push_back(string_pair_2_bool("", "a", false));
  vec.push_back(string_pair_2_bool("", "abc", false));
  vec.push_back(string_pair_2_bool("a", "", false));
  vec.push_back(string_pair_2_bool("a", "a", true));
  vec.push_back(string_pair_2_bool("aaa", "a", true));
  vec.push_back(string_pair_2_bool("abc", "xyz", false));
  vec.push_back(string_pair_2_bool("abc", "xcz", true));
  vec.push_back(string_pair_2_bool("a☺b☻c☹d", "xcz", true));
  vec.push_back(string_pair_2_bool("aRegExp*", ".(|)*+?^$[]", true));
  vec.push_back(string_pair_2_bool(dots+dots+dots, " ", false));

  for (auto &item : vec) {
    assert(chef::strings::contains_any(item.pair_[0], item.pair_[1]) == item.result_);
  }
}

void strings_has_prifex_test() {
  std::vector<string_pair_2_bool> vec;
  vec.push_back(string_pair_2_bool("abc", "bc", false));
  vec.push_back(string_pair_2_bool("abc", "bcd", false));
  vec.push_back(string_pair_2_bool("", "a", false));
  vec.push_back(string_pair_2_bool("abc", "a", true));
  vec.push_back(string_pair_2_bool("abc", "ab", true));
  vec.push_back(string_pair_2_bool("abc", "", true));

  for (auto &item : vec) {
    assert(chef::strings::has_prefix(item.pair_[0], item.pair_[1]) == item.result_);
  }
}

void strings_has_suffix_test() {
  std::vector<string_pair_2_bool> vec;
  vec.push_back(string_pair_2_bool("abc", "ab", false));
  vec.push_back(string_pair_2_bool("abc", "bcd", false));
  vec.push_back(string_pair_2_bool("", "a", false));
  vec.push_back(string_pair_2_bool("abc", "c", true));
  vec.push_back(string_pair_2_bool("abc", "bc", true));
  vec.push_back(string_pair_2_bool("abc", "", true));

  for (auto &item : vec) {
    assert(chef::strings::has_suffix(item.pair_[0], item.pair_[1]) == item.result_);
  }
}

void strings_count_test() {
  std::vector<string_pair_2_int> vec;
  vec.push_back(string_pair_2_int("", "", 1));
  vec.push_back(string_pair_2_int("", "notempty", 0));
  vec.push_back(string_pair_2_int("notempty", "", 0));
  vec.push_back(string_pair_2_int("smaller", "not smaller", 0));
  vec.push_back(string_pair_2_int("12345678987654321", "6", 2));
  vec.push_back(string_pair_2_int("611161116", "6", 3));
  vec.push_back(string_pair_2_int("notequal", "NotEqual", 0));
  vec.push_back(string_pair_2_int("equal", "equal", 1));
  vec.push_back(string_pair_2_int("abc1231231123q", "123", 3));
  vec.push_back(string_pair_2_int("11111", "11", 2));

  for (auto &item : vec) {
    assert(chef::strings::count(item.pair_[0], item.pair_[1]) == item.result_);
  }
}

void strings_to_lower_test() {
  assert(chef::strings::to_lower("") == "");
  assert(chef::strings::to_lower("1") == "1");
  assert(chef::strings::to_lower("a") == "a");
  assert(chef::strings::to_lower("a1") == "a1");
  assert(chef::strings::to_lower("A") == "a");
  assert(chef::strings::to_lower("aA") == "aa");
  assert(chef::strings::to_lower("aAbbAc") == "aabbac");
  assert(chef::strings::to_lower("!aAbbAc~") == "!aabbac~");
}

void strings_to_upper_test() {
  assert(chef::strings::to_lower("") == "");
  assert(chef::strings::to_lower("1") == "1");
  assert(chef::strings::to_lower("a") == "A");
  assert(chef::strings::to_lower("a1") == "A1");
  assert(chef::strings::to_lower("A") == "A");
  assert(chef::strings::to_lower("aA") == "AA");
  assert(chef::strings::to_lower("aAbbAc") == "AABBAC");
  assert(chef::strings::to_lower("!aAb&bAc~") == "!aab&bac~");
}


void strings_join_test() {
  std::string delimiter = "---";
  std::vector<std::string> array;
  assert(chef::strings::join(array, delimiter) == std::string());
  array.push_back("test");
  assert(chef::strings::join(array, delimiter) == std::string("test"));
  array.push_back("hello");
  assert(chef::strings::join(array, delimiter) == std::string("test---hello"));
  array.push_back("world");
  assert(chef::strings::join(array, delimiter) == std::string("test---hello---world"));
}

void strings_split_test() {
  assert(chef::strings::split(std::string(), '-').empty());
  std::string str1 = "a";
  std::vector<std::string> res1 = chef::strings::split(str1, '-');
  assert(res1.size() == 1);
  assert(res1[0] == "a");

  std::string str2 = "a-";
  std::vector<std::string> res2 = chef::strings::split(str2, '-');
  assert(res2.size() == 1);
  assert(res2[0] == "a");

  std::string str3 = "-a";
  std::vector<std::string> res3 = chef::strings::split(str3, '-');
  assert(res3.size() == 2);
  assert(res3[0] == "");
  assert(res3[1] == "a");

  std::string str4 = "a*bc";
  std::vector<std::string> res4 = chef::strings::split(str4, '*');
  assert(res4.size() == 2);
  assert(res4[0] == "a");
  assert(res4[1] == "bc");

  std::string str5 = "--";
  std::vector<std::string> res5 = chef::strings::split(str5, '-');
  assert(res5.size() == 2);
  assert(res5[0] == "");
  assert(res5[1] == "");
}

void strings_trim_left_test() {
  std::vector<std::vector<std::string> > vec = {
    {"", "", ""},
    {"a", "", "a"},
    {"", "b", ""},
    {"a", "b", "a"},
    {"a", "a", ""},
    {"ab", "a", "b"},
    {"ba", "a", "ba"},
    {"ba", "ac", "ba"},
    {"ba", "ab", ""},
    {"ba", "abc", ""},
    {"bca", "abc", ""},
    {"bca", "acd", "bca"},
    {"bca", "bcd", "a"},
    {"cbbbccacd", "bcd", "acd"},
  };
  for (auto &item : vec) {
    assert(chef::strings::trim_left(item[0], item[1]) == item[2]);
  }
}

void strings_trim_right_test() {
  std::vector<std::vector<std::string> > vec = {
    {"", "", ""},
    {"a", "", "a"},
    {"", "b", ""},
    {"a", "b", "a"},
    {"a", "a", ""},
    {"ab", "a", "ab"},
    {"ba", "a", "b"},
    {"ba", "ac", "b"},
    {"ba", "ab", ""},
    {"ba", "abc", ""},
    {"bca", "abc", ""},
    {"bca", "acd", "b"},
    {"bca", "bcd", "bca"},
    {"cbbbccacd", "bcd", "cbbbcca"},
  };
  for (auto &item : vec) {
    assert(chef::strings::trim_right(item[0], item[1]) == item[2]);
  }
}

void strings_trim_prefix_test() {
  std::vector<std::vector<std::string> > vec = {
    {"", "", ""},
    {"a", "", "a"},
    {"", "b", ""},
    {"a", "b", "a"},
    {"a", "a", ""},
    {"ab", "a", "b"},
    {"ba", "a", "ba"},
    {"ba", "ac", "ba"},
    {"ba", "ab", "ba"},
    {"ba", "abc", "ba"},
    {"bca", "abc", "bca"},
    {"bca", "acd", "bca"},
    {"bca", "bcd", "bca"},
    {"cbbbccacd", "bcd", "cbbbccacd"},
    {"bca", "b", "ca"},
    {"bca", "bc", "a"},
    {"bca", "bca", ""},
    {"bca", "bcd", "bca"},
  };
  for (auto &item : vec) {
    assert(chef::strings::trim_prefix(item[0], item[1]) == item[2]);
  }
}

void strings_trim_suffix_test() {
  std::vector<std::vector<std::string> > vec = {
    {"", "", ""},
    {"a", "", "a"},
    {"", "b", ""},
    {"a", "b", "a"},
    {"a", "a", ""},
    {"a", "ab", "a"},
    {"ab", "a", "ab"},
    {"ba", "a", "b"},
    {"ba", "ac", "ba"},
    {"ba", "ab", "ba"},
    {"ba", "abc", "ba"},
    {"bca", "abc", "bca"},
    {"bca", "acd", "bca"},
    {"bca", "bcd", "bca"},
    {"cbbbccacd", "bcd", "cbbbccacd"},
    {"bca", "b", "bca"},
    {"bca", "bc", "bca"},
    {"bca", "bca", ""},
    {"bca", "bcd", "bca"},
  };

  for (auto &item : vec) {
    assert(chef::strings::trim_suffix(item[0], item[1]) == item[2]);
  }
}

int main() {
  ENTER_TEST;

  strings_compare_test();
  strings_contains_test();
  strings_contains_any_test();
  strings_has_prifex_test();
  strings_has_suffix_test();
  strings_count_test();
  strings_to_lower_test();
  strings_join_test();
  strings_split_test();
  strings_trim_left_test();
  strings_trim_right_test();
  strings_trim_prefix_test();
  strings_trim_suffix_test();

  return 0;
}
