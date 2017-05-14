#!/usr/bin/env bash
# @file   valgrind.sh
# @author
#   chef <191201771@qq.com>
#     -created 2017-05-14 11:07:59
#     -initial release xxxx-xx-xx
# @brief
#   跑valgrind内存测试

. ./common.sh

cd ${BIN_DIR}

valgrind --leak-check=full --show-leak-kinds=all ./buffer_test
valgrind --leak-check=full --show-leak-kinds=all ./chunk_head_op_test
valgrind --leak-check=full --show-leak-kinds=all ./crypto_md5_op_test
valgrind --leak-check=full --show-leak-kinds=all ./daemon_op_test
valgrind --leak-check=full --show-leak-kinds=all ./dump_test
valgrind --leak-check=full --show-leak-kinds=all ./encoding_base64_op_test
valgrind --leak-check=full --show-leak-kinds=all ./filepath_op_test
valgrind --leak-check=full --show-leak-kinds=all ./hash_test
valgrind --leak-check=full --show-leak-kinds=all ./json_config_test
valgrind --leak-check=full --show-leak-kinds=all ./noncopyable_test
valgrind --leak-check=full --show-leak-kinds=all ./os_exec_op_test
valgrind --leak-check=full --show-leak-kinds=all ./strings_test
valgrind --leak-check=full --show-leak-kinds=all ./stuff_test
valgrind --leak-check=full --show-leak-kinds=all ./task_thread_test
valgrind --leak-check=full --show-leak-kinds=all ./this_machine_op_test
valgrind --leak-check=full --show-leak-kinds=all ./this_proc_op_test
valgrind --leak-check=full --show-leak-kinds=all ./this_thread_op_test
valgrind --leak-check=full --show-leak-kinds=all ./thread_pool_test
valgrind --leak-check=full --show-leak-kinds=all ./wait_event_test

#程序退出时不做保证
#valgrind --leak-check=full --show-leak-kinds=all ./redis_backend_test
#valgrind --leak-check=full --show-leak-kinds=all ./log_test
