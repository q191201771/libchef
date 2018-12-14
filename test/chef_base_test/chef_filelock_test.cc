#include "chef_base/chef_filelock.hpp"
#include "./common/assert_wrapper.hpp"
#include "./common/check_log.hpp"
#include <string.h>

static void example() {

}

int main(int argc, char **argv) {
  ENTER_TEST;

  example();

  bool first = true;
  if (argc == 2 && strcmp(argv[1], "2") == 0) {
    first = false;
  }

  bool bret;
  chef::filelock fl;

  printf("> attach...\n");
  bret = fl.attach("/tmp/chef_filelock_test.lock");
  printf("< attach %d...\n", bret);
  assert(bret);

  //assert(fl.unlock());
  //assert(fl.lock());
  //assert(fl.lock());
  //printf("< lock\n");
  //sleep(5);
  //printf("> unlock\n");
  //assert(fl.unlock());
  //assert(fl.unlock());
  //assert(fl.unlock());
  //printf("done\n");
  //sleep(-1);

  printf("> try_lock...\n");
  bret = fl.try_lock();
  assert(first ? bret: !bret);
  printf("< try_lock %d...\n", bret);

  printf("> lock...\n");
  bret = fl.lock();
  assert(bret);
  printf("< lock %d...\n", bret);

  // 本filelock对象锁可重入
  printf("> lock...\n");
  bret = fl.lock();
  assert(bret);
  printf("< lock %d...\n", bret);

  printf("> try_lock...\n");
  bret = fl.try_lock();
  assert(bret);
  printf("< try_lock %d...\n", bret);


  printf("> sleep...\n");
  sleep(3);
  printf("< sleep...\n");

  printf("> unlock...\n");
  bret = fl.unlock();
  assert(bret);
  printf("< unlock %d...\n", bret);
  return 0;
}
