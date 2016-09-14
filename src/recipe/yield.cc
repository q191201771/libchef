#include <iostream>
#include <chrono>
#include <thread>

// "busy sleep" while suggesting that other threads run
// for a small amount of time
void little_sleep(std::chrono::microseconds us)
{
  auto count = 0;
  auto max = -1;
  auto start = std::chrono::high_resolution_clock::now();
  auto end = start + us;
  do {
    auto yield_start = std::chrono::high_resolution_clock::now();
    std::this_thread::yield();
    auto elapsed = std::chrono::high_resolution_clock::now() - yield_start;
    auto duration = std::chrono::duration_cast<std::chrono::microseconds>(elapsed).count();
    std::cout << "yield elapsed " << duration << " microseconds\n";
    count++;
    if (duration > max) {
      max = duration;
    }
  } while (std::chrono::high_resolution_clock::now() < end);
  std::cout << "count " << count << " " << "max " << max << "\n";
}

int main()
{
  auto start = std::chrono::high_resolution_clock::now();

  little_sleep(std::chrono::microseconds(1000 * 1000));

  auto elapsed = std::chrono::high_resolution_clock::now() - start;
  std::cout << "waited for "
    << std::chrono::duration_cast<std::chrono::microseconds>(elapsed).count()
    << " microseconds\n";

  return 0;
}
