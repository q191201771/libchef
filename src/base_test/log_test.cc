#include "../base/log.h"

int main() {
  chef::log::init(chef::log::mode_debug);

  printf("Check log done.\n");
  return 0;
}
