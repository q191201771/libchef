#include "../base/http_op.h"
#include "./common/check_log.hpp"
#include "./common/assert_wrapper.hpp"
#include <stdio.h>

int main() {
    ENTER_TEST;

    int ret;
    chef::http_op::response resp;

    ret = chef::http_op::global_init_curl();
    assert(ret == 0);
    std::string url = "www.baidu.com";
    std::map<std::string, std::string> headers;
    headers["fuck"] = "you";
    headers["User-Agent"] = "Mozilla/5.0 (Macintosh; Intel Mac OS X 10_11_6) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/54.0.2840.98 Safari/537.36";

    ret = chef::http_op::get(url, &headers, 10000, resp);
    assert(ret == 0 && resp.code_ == 200 && !resp.content_.empty());
    printf("headers:------\n");
    std::map<std::string, std::string>::iterator iter = resp.headers_.begin();
    for (; iter != resp.headers_.end(); iter++) {
      printf("[%s]:[%s]\n", iter->first.c_str(), iter->second.c_str());
    }
    printf("body:-----\n%lu\n", resp.content_.length());

    return 0;
}
