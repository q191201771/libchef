#include <stdio.h>
#include <assert.h>
#include <stdint.h>
#include <set>
#include <vector>
#include <random>
#include <thread>
#include <atomic>

/**
 * 用这个demo证明volatile并不能保证整型的原子操作。
 *
 * 测试用例：用<THREAD_NUM>个线程对<int32_val>变量写入0和0x7FFFFFFF时，在另外<THREAD_NUM>个线程中读<int32_val>的值，看是否会出现
 *         0和0x7FFFFFFF以外的值。
 *
 * 测试结果：确实会出现脏值。
 *
 * 所以用atomic或mutex才是王道。
 *
 */

const static int32_t INT32_ARRAY[] = {0x0, 0x7FFFFFFF};
const static int64_t INT64_ARRAY[] = {0x0, 0x7FFFFFFFFFFFFFFF};

volatile int32_t int32_val = INT32_ARRAY[0];
volatile int64_t int64_val = INT64_ARRAY[0];

const static int THREAD_NUM = 16; /// 读线程和写线程加起来等于THREAD_NUM * 2
const static int64_t LOOP_COUNT= 1 * 1024 * 1024;

std::atomic<int64_t> index_0_count{0};
std::atomic<int64_t> index_1_count{0};

int main() {
    std::vector<std::shared_ptr<std::thread> > set_threads;
    std::vector<std::shared_ptr<std::thread> > get_threads;

    for (int i = 0; i < THREAD_NUM; i++) {
        set_threads.push_back(std::make_shared<std::thread>([i]() {
            for (int64_t j = 0; j < LOOP_COUNT; j++) {
                int32_val = (i % 2) ? INT32_ARRAY[0] : INT32_ARRAY[1];
                int64_val = (i % 2) ? INT64_ARRAY[0] : INT64_ARRAY[1];
            }
        }));

        get_threads.push_back(std::make_shared<std::thread>([i]() {
            for (int64_t j = 0; j < LOOP_COUNT; j++) {
                int32_t int32_val_ninja = int32_val;
                int64_t int64_val_ninja = int64_val;
                // assert(int32_val_ninja == INT32_ARRAY[0] || int32_val_ninja == INT32_ARRAY[1]);
                // assert(int64_val_ninja == INT64_ARRAY[0] || int64_val_ninja == INT64_ARRAY[1]);
                if ((int32_val_ninja != INT32_ARRAY[0] && int32_val_ninja != INT32_ARRAY[1]) ||
                    (int64_val_ninja != INT64_ARRAY[0] && int64_val_ninja != INT64_ARRAY[1])
                ) {
                    /// 移除这段注释
                    /// 这里打印出来可能依然是0或0x7FFFFFFF，因为检查到int32_val_ninja脏了以后，再到打印的时候，可能又写回正确值了。
                    /// 很遗憾，我测试的时候，尽管几乎每次都能assert到bad case，但是并没能打出脏数值。
                    printf("bad case!!! i:%d j:%ld int32_val_ninja:%d int64_val_ninja:%ld\n", i, j, int32_val_ninja, int64_val_ninja);
                    // exit(-1);

                    /// try to take a guess
                    switch (int32_val_ninja) {
                    case     0x000F: printf("int32_val_ninja == 0x000F\n"); break;
                    case     0x00FF: printf("int32_val_ninja == 0x00FF\n"); break;
                    case     0x0FFF: printf("int32_val_ninja == 0x0FFF\n"); break;
                    case     0xFFFF: printf("int32_val_ninja == 0xFFFF\n"); break;
                    case    0xFFFFF: printf("int32_val_ninja == 0xFFFFF\n"); break;
                    case   0xFFFFFF: printf("int32_val_ninja == 0xFFFFFF\n"); break;
                    case  0xFFFFFFF: printf("int32_val_ninja == 0xFFFFFFF\n"); break;
                    case 0x7FFF0000: printf("int32_val_ninja == 0x7FFF0000\n"); break;
                    case 0xFFFFFFFF: printf("int32_val_ninja == 0xFFFFFFFF\n"); break;
                    };
                    switch (int64_val_ninja) {
                    case             0x000F: printf("int64_val_ninja == 0xFFFF\n"); break;
                    case             0x00FF: printf("int64_val_ninja == 0xFFFF\n"); break;
                    case             0x0FFF: printf("int64_val_ninja == 0xFFFF\n"); break;
                    case             0xFFFF: printf("int64_val_ninja == 0xFFFF\n"); break;
                    case         0x7FFF0000: printf("int64_val_ninja == 0x7FFF0000\n"); break;
                    case         0x7FFFFFFF: printf("int64_val_ninja == 0xFFFFFFFF\n"); break;
                    case         0xFFFFFFFF: printf("int64_val_ninja == 0xFFFFFFFF\n"); break;
                    case 0x7FFFFFFF00000000: printf("int64_val_ninja == 0x7FFFFFFF00000000\n"); break;
                    case 0xFFFFFFFF00000000: printf("int64_val_ninja == 0xFFFFFFFF00000000\n"); break;
                    case 0xFFFFFFFFFFFFFFFF: printf("int64_val_ninja == 0xFFFFFFFFFFFFFFFF\n"); break;
                    };
                }
            }
        }));
    }

    for (int i = 0; i < THREAD_NUM; i++) {
        set_threads[i]->join();
        get_threads[i]->join();
    }

    // printf("stat. index_0_count:%ld index_1_count:%ld sum:%ld total:%ld bad:%ld\n",
    //        index_0_count.load(), index_1_count.load(), index_0_count.load()+index_1_count.load(), THREAD_NUM*LOOP_COUNT,
    //        THREAD_NUM*LOOP_COUNT - index_0_count.load() - index_1_count.load());

    printf("bye.\n");
    return 0;
}
