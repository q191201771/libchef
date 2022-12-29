#include "./common/assert_wrapper.hpp"
#include "./common/check_log.hpp"
#include "chef_base/chef_url.hpp"

int main() {
  ENTER_TEST;

  chef::url_context ctx;
  int res = chef::url_context::parse("http://127.0.0.1:80/live/test110.flv?a=1&b=2", ctx);
  chef::url_context expect;
  assert(res == 0);
  assert(ctx.url_ == "http://127.0.0.1:80/live/test110.flv?a=1&b=2");
  assert(ctx.scheme_ == "http");
  assert(ctx.stdhost_ == "127.0.0.1:80");
  assert(ctx.host_ == "127.0.0.1");
  assert(ctx.origin_port_ == 80);
  assert(ctx.path_with_raw_query_ == "/live/test110.flv?a=1&b=2");
  assert(ctx.path_ == "/live/test110.flv");
  assert(ctx.path_without_last_item_ == "live");
  assert(ctx.last_item_of_path_ == "test110.flv");
  assert(ctx.raw_query_ == "a=1&b=2");
}
