#include "../chef_base/chef_http_op.hpp"
#include "./common/check_log.hpp"
#include "./common/assert_wrapper.hpp"
#include <stdio.h>

int main() {
    ENTER_TEST;

    int ret;
    chef::http_op::response resp;

    //ret = chef::http_op::global_init_curl();
    //assert(ret == 0);
    std::string url = "www.taobao.com";
    std::map<std::string, std::string> headers;
    headers["fuck"]       = "you";
    headers["User-Agent"] = "Mozilla/5.0 (Macintosh; Intel Mac OS X 10_11_6) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/54.0.2840.98 Safari/537.36";
    std::map<std::string, std::string> cookies;
    cookies["BAIDUID"]  = "746D850D8998853CEE879633364B4888";
    cookies["BIDUPSID"] = "746D850D8998853CEE879633364B4889";

    ret = chef::http_op::get_or_post(url, &headers, &cookies, NULL, 10000, resp);
    assert(ret == 0 && resp.code_ == 200 && !resp.content_.empty());
    printf("ret:%d,code:%d\n", ret, resp.code_);
    printf("headers:------\n");
    std::map<std::string, std::string>::iterator iter = resp.headers_.begin();
    for (; iter != resp.headers_.end(); iter++) {
      printf("[%s]:[%s]\n", iter->first.c_str(), iter->second.c_str());
    }
    printf("cookies:------\n");
    std::map<std::string, chef::http_op::response_cookie>::iterator iter2 = resp.cookies_.begin();
    for (; iter2 != resp.cookies_.end(); iter2++) {
      printf("[%s]:[%s]\n", iter2->first.c_str(), iter2->second.stringify().c_str());
    }
    printf("body:-----\n%lu\n", resp.content_.length());

    return 0;
}
