#include <iostream>
#include <chrono>
#include <thread>

int main()
{
  using namespace std::chrono_literals;
  std::cout << "Hello waiter" << std::endl;
  auto start = std::chrono::high_resolution_clock::now();
  std::this_thread::sleep_for(2s);
  std::this_thread::sleep_for(std::chrono::seconds(2));
  std::this_thread::sleep_for(200ms);
  std::this_thread::sleep_for(std::chrono::milliseconds(200));
  auto end = std::chrono::high_resolution_clock::now();
  std::chrono::duration<double, std::milli> elapsed = end-start;
  std::cout << "Waited " << elapsed.count() << " ms\n";

  return 0;
}
