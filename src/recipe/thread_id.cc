#include <iostream>
#include <thread>
#include <chrono>
#include <string>

void quick() {
  /// noop
}
 
// void slow(int sec = 3) {
//   std::this_thread::sleep_for(std::chrono::seconds(sec));
// }

int main() {
  {
    std::thread t1;
    std::thread t2;

    /// stdout -> t1 id: thread::id of a non-executing thread
    std::cout << "t1 id: " << t1.get_id() << std::endl;

    /// stdout -> t1 == t2: true
    std::cout << "t1 == t2: " << (t1.get_id() == t2.get_id() ? "true" : "false") << std::endl;
  }

  {
    std::thread t1(quick);
    t1.join();

    /// stdout -> t1 id: thread::id of a non-executing thread
    std::cout << "t1 id: " << t1.get_id() << std::endl;
  }

  return 0;
}
