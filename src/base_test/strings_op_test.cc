#include "../base/strings_op.h"
#include <stdio.h>
#include <stdint.h>
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
  /// std::string_op::compare返回值为 0 || > 0 || < 0，而不是 0 || 1 || -1
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
      assert(chef::strings_op::compare(item.pair_[0], item.pair_[1]) == 0);
      break;
    case POSITIVE_FLAG:
      assert(chef::strings_op::compare(item.pair_[0], item.pair_[1]) > 0);
      break;
    case NEGITIVE_FLAG:
      assert(chef::strings_op::compare(item.pair_[0], item.pair_[1]) < 0);
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
    assert(chef::strings_op::contains(item.pair_[0], item.pair_[1]) == item.result_);
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
    assert(chef::strings_op::contains_any(item.pair_[0], item.pair_[1]) == item.result_);
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
    assert(chef::strings_op::has_prefix(item.pair_[0], item.pair_[1]) == item.result_);
  }
}

void strings_has_suffix_test() {
  std::vector<string_pair_2_bool> vec;
  vec.push_back(string_pair_2_bool("abc", "ab", false));
  vec.push_back(string_pair_2_bool("abc", "bcd", false));
  vec.push_back(string_pair_2_bool("", "a", false));
  vec.push_back(string_pair_2_bool("abc", "c", true));
  vec.push_back(string_pair_2_bool("abc", "bc", true));
  vec.push_back(string_pair_2_bool("abc", "", false));

  for (auto &item : vec) {
    assert(chef::strings_op::has_suffix(item.pair_[0], item.pair_[1]) == item.result_);
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
    assert(chef::strings_op::count(item.pair_[0], item.pair_[1]) == item.result_);
  }
}

void strings_to_lower_test() {
  assert(chef::strings_op::to_lower("") == "");
  assert(chef::strings_op::to_lower("1") == "1");
  assert(chef::strings_op::to_lower("a") == "a");
  assert(chef::strings_op::to_lower("a1") == "a1");
  assert(chef::strings_op::to_lower("A") == "a");
  assert(chef::strings_op::to_lower("aA") == "aa");
  assert(chef::strings_op::to_lower("aAbbAc") == "aabbac");
  assert(chef::strings_op::to_lower("!aAbbAc~") == "!aabbac~");
}

void strings_to_upper_test() {
  assert(chef::strings_op::to_lower("") == "");
  assert(chef::strings_op::to_lower("1") == "1");
  assert(chef::strings_op::to_lower("a") == "A");
  assert(chef::strings_op::to_lower("a1") == "A1");
  assert(chef::strings_op::to_lower("A") == "A");
  assert(chef::strings_op::to_lower("aA") == "AA");
  assert(chef::strings_op::to_lower("aAbbAc") == "AABBAC");
  assert(chef::strings_op::to_lower("!aAb&bAc~") == "!aab&bac~");
}


void strings_join_test() {
  std::string delimiter = "---";
  std::vector<std::string> array;
  assert(chef::strings_op::join(array, delimiter) == std::string());
  array.push_back(std::string());
  assert(chef::strings_op::join(array, delimiter) == std::string(""));
  array.push_back("test");
  assert(chef::strings_op::join(array, delimiter) == std::string("---test"));
  array.push_back("hello");
  assert(chef::strings_op::join(array, delimiter) == std::string("---test---hello"));
  array.push_back("world");
  assert(chef::strings_op::join(array, delimiter) == std::string("---test---hello---world"));
}

void strings_split_any_test() {
  std::vector<std::string> res;

  assert(chef::strings_op::split_any("", "-*").empty());
  assert(chef::strings_op::split_any("", "-*", false).empty());

  res = chef::strings_op::split_any("ab", "");
  assert(res.size() == 1);
  assert(res[0] == "ab");
  res = chef::strings_op::split_any("ab", "", false);
  assert(res.size() == 1);
  assert(res[0] == "ab");

  res = chef::strings_op::split_any("-a1*b22", "*-");
  assert(res.size() == 3);
  assert(res[0] == "");
  assert(res[1] == "a1");
  assert(res[2] == "b22");
  res = chef::strings_op::split_any("-a1*b22", "*-", false);
  assert(res.size() == 2);
  assert(res[0] == "a1");
  assert(res[1] == "b22");

  res = chef::strings_op::split_any("a*b223-", "*-");
  assert(res.size() == 3);
  assert(res[0] == "a");
  assert(res[1] == "b223");
  assert(res[2] == "");
  res = chef::strings_op::split_any("a*b223-", "*-", false);
  assert(res.size() == 2);
  assert(res[0] == "a");
  assert(res[1] == "b223");

  res = chef::strings_op::split_any("a111*-b", "*-");
  assert(res.size() == 3);
  assert(res[0] == "a111");
  assert(res[1] == "");
  assert(res[2] == "b");
  res = chef::strings_op::split_any("a111*-b", "*-", false);
  assert(res.size() == 2);
  assert(res[0] == "a111");
  assert(res[1] == "b");

  res = chef::strings_op::split_any("a111|ccc|ddd=b", "|=");
  assert(res.size() == 4);
  assert(res[0] == "a111");
  assert(res[1] == "ccc");
  assert(res[2] == "ddd");
  assert(res[3] == "b");
  res = chef::strings_op::split_any("a111|ccc|ddd=b", "|=-", false);
  assert(res.size() == 4);
  assert(res[0] == "a111");
  assert(res[1] == "ccc");
  assert(res[2] == "ddd");
  assert(res[3] == "b");

  res = chef::strings_op::split_any("a111|ccc|ddd=b", "|");
  assert(res.size() == 3);
  assert(res[0] == "a111");
  assert(res[1] == "ccc");
  assert(res[2] == "ddd=b");
}

void strings_split_chars_test() {
  std::vector<std::string> res;

  assert(chef::strings_op::split(std::string(), "-").empty());
  assert(chef::strings_op::split(std::string(), "-", false).empty());
  assert(chef::strings_op::split(std::string(), "--").empty());
  assert(chef::strings_op::split(std::string(), "--", false).empty());

  res = chef::strings_op::split("a", "-");
  assert(res.size() == 1);
  assert(res[0] == "a");
  res = chef::strings_op::split("a", "--", false);
  assert(res.size() == 1);
  assert(res[0] == "a");

  res = chef::strings_op::split("b1--", "--");
  assert(res.size() == 2);
  assert(res[0] == "b1");
  assert(res[1] == "");
  res = chef::strings_op::split("b1--", "--", false);
  assert(res.size() == 1);
  assert(res[0] == "b1");

  res = chef::strings_op::split("--c22", "--");
  assert(res.size() == 2);
  assert(res[0] == "");
  assert(res[1] == "c22");
  res = chef::strings_op::split("--c22", "--", false);
  assert(res.size() == 1);
  assert(res[0] == "c22");

  res = chef::strings_op::split("d333**e4444", "**");
  assert(res.size() == 2);
  assert(res[0] == "d333");
  assert(res[1] == "e4444");
  res = chef::strings_op::split("d333**e4444", "**", false);
  assert(res.size() == 2);
  assert(res[0] == "d333");
  assert(res[1] == "e4444");

  res = chef::strings_op::split("f55555--g4444--", "--");
  assert(res.size() == 3);
  assert(res[0] == "f55555");
  assert(res[1] == "g4444");
  assert(res[2] == "");
  res = chef::strings_op::split("f55555--g4444--", "--", false);
  assert(res.size() == 2);
  assert(res[0] == "f55555");
  assert(res[1] == "g4444");

  res = chef::strings_op::split("h333---i22--j1", "--");
  assert(res.size() == 4);
  assert(res[0] == "h333");
  assert(res[1] == "");
  assert(res[2] == "-i22");
  assert(res[3] == "j1");
  res = chef::strings_op::split("h333---i22--j1", "--", false);
  assert(res.size() == 3);
  assert(res[0] == "h333");
  assert(res[1] == "-i22");
  assert(res[2] == "j1");

  res = chef::strings_op::split("----", "--");
  assert(res.size() == 3);
  assert(res[0] == "");
  assert(res[1] == "");
  assert(res[2] == "");
  res = chef::strings_op::split("----", "--", false);
  assert(res.size() == 0);
}

void strings_split_test() {
  std::vector<std::string> res;

  assert(chef::strings_op::split(std::string(), '-').empty());
  assert(chef::strings_op::split(std::string(), '-', false).empty());

  res = chef::strings_op::split("a", '-');
  assert(res.size() == 1);
  assert(res[0] == "a");
  res = chef::strings_op::split("a", '-', false);
  assert(res.size() == 1);
  assert(res[0] == "a");

  res = chef::strings_op::split("b1-", '-');
  assert(res.size() == 2);
  assert(res[0] == "b1");
  assert(res[1] == "");
  res = chef::strings_op::split("b1-", '-', false);
  assert(res.size() == 1);
  assert(res[0] == "b1");

  res = chef::strings_op::split("-c22", '-');
  assert(res.size() == 2);
  assert(res[0] == "");
  assert(res[1] == "c22");
  res = chef::strings_op::split("-c22", '-', false);
  assert(res.size() == 1);
  assert(res[0] == "c22");

  res = chef::strings_op::split("d333*e4444", '*');
  assert(res.size() == 2);
  assert(res[0] == "d333");
  assert(res[1] == "e4444");
  res = chef::strings_op::split("d333*e4444", '*', false);
  assert(res.size() == 2);
  assert(res[0] == "d333");
  assert(res[1] == "e4444");

  res = chef::strings_op::split("f55555-g4444-", '-');
  assert(res.size() == 3);
  assert(res[0] == "f55555");
  assert(res[1] == "g4444");
  assert(res[2] == "");
  res = chef::strings_op::split("f55555-g4444-", '-', false);
  assert(res.size() == 2);
  assert(res[0] == "f55555");
  assert(res[1] == "g4444");

  res = chef::strings_op::split("h333--i22-j1", '-');
  assert(res.size() == 4);
  assert(res[0] == "h333");
  assert(res[1] == "");
  assert(res[2] == "i22");
  assert(res[3] == "j1");
  res = chef::strings_op::split("h333--i22-j1", '-', false);
  assert(res.size() == 3);
  assert(res[0] == "h333");
  assert(res[1] == "i22");
  assert(res[2] == "j1");

  res = chef::strings_op::split("--", '-');
  assert(res.size() == 3);
  assert(res[0] == "");
  assert(res[1] == "");
  assert(res[2] == "");
  res = chef::strings_op::split("--", '-', false);
  assert(res.size() == 0);
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
    assert(chef::strings_op::trim_left(item[0], item[1]) == item[2]);
  }
  assert(chef::strings_op::trim_left(" \taa b  \t") == "aa b  \t");
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
    assert(chef::strings_op::trim_right(item[0], item[1]) == item[2]);
  }
  assert(chef::strings_op::trim_right(" \taa b  \t") == " \taa b");
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
    assert(chef::strings_op::trim_prefix(item[0], item[1]) == item[2]);
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
    assert(chef::strings_op::trim_suffix(item[0], item[1]) == item[2]);
  }
}

void strings_splitlines_test() {
  std::vector<std::string> lines;
  lines = chef::strings_op::splitlines("", true);
  assert(lines.size() == 0);
  lines = chef::strings_op::splitlines("", false);
  assert(lines.size() == 0);

  lines = chef::strings_op::splitlines("\r", false);
  assert(lines.size() == 1 && lines[0].empty());
  lines = chef::strings_op::splitlines("\n", false);
  assert(lines.size() == 1 && lines[0].empty());
  lines = chef::strings_op::splitlines("\r\n", false);
  assert(lines.size() == 1 && lines[0].empty());
  lines = chef::strings_op::splitlines("\r", true);
  assert(lines.size() == 1 && lines[0] == "\r");
  lines = chef::strings_op::splitlines("\n", true);
  assert(lines.size() == 1 && lines[0] == "\n");
  lines = chef::strings_op::splitlines("\r\n", true);
  assert(lines.size() == 1 && lines[0] == "\r\n");

  lines = chef::strings_op::splitlines("ab c\n\nde fg\rkl\r\n", false);
  assert(lines.size() == 4 && lines[0] == "ab c" && lines[1] == "" && lines[2] == "de fg" && lines[3] == "kl");
  lines = chef::strings_op::splitlines("ab c\n\nde fg\rkl\r\n", true);
  assert(lines.size() == 4 && lines[0] == "ab c\n" && lines[1] == "\n" && lines[2] == "de fg\r" && lines[3] == "kl\r\n");
}

void strings_to_string_test() {
  int a         = 1;
  float b       = 2.3f;
  double c      = 3.4;
  char d        = 'a';
  const char *e = "hello";
  std::string f = "world";
  uint32_t g    = 5;
  assert(chef::strings_op::to_string(a) == "1");
  assert(chef::strings_op::to_string(b) == "2.3");
  assert(chef::strings_op::to_string(c) == "3.4");
  assert(chef::strings_op::to_string(d) == "a");
  assert(chef::strings_op::to_string(e) == "hello");
  assert(chef::strings_op::to_string(f) == "world");
  assert(chef::strings_op::to_string(g) == "5");
}

void strings_replace_test() {
  std::string s;
  std::string target;
  std::string replacement;
  assert(chef::strings_op::replace("", "aaa", "bbb") == "");
  assert(chef::strings_op::replace("aaa", "", "bbb") == "aaa");
  assert(chef::strings_op::replace("aba", "b", "") == "aa");
  assert(chef::strings_op::replace("aa", "aa", "c") == "c");
  assert(chef::strings_op::replace("aaa", "aa", "c") == "ca");
  assert(chef::strings_op::replace("aaaa", "aa", "c") == "cc");
  assert(chef::strings_op::replace("aaaa", "aaa", "c") == "ca");
  assert(chef::strings_op::replace("aaaaa", "aa", "c") == "cca");
  assert(chef::strings_op::replace("aaaaa", "aaa", "c") == "caa");
  assert(chef::strings_op::replace("aabacaada", "aa", "c") == "cbaccda");
  assert(chef::strings_op::replace("abacaadaa", "aa", "c") == "abaccdc");
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
  strings_splitlines_test();
  strings_to_string_test();
  strings_replace_test();
  strings_split_any_test();

  return 0;
}
