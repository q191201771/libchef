#!/usr/bin/env bash
# @file   unit_test.sh
# @author
#   chef <191201771@qq.com>
#     -created 2017-05-14 10:41:38
#     -initial release xxxx-xx-xx
# @brief
#   跑单元测试

. ./common.sh

cd ${SCONS_BIN_DIR}

./buffer_test && ./chef_count_dump_test && ./filepath_op_test && ./noncopyable_test && ./strings_op_test && ./task_thread_test &&
./thread_pool_test && ./wait_event_test && ./wait_event_counter_test
./chunk_head_op_test
./daemon_op_test
./hash_test
./json_config_test
./log_test
./os_exec_op_test
./stuff_test
./this_machine_op_test
./this_proc_op_test
./this_thread_op_test
./macro_test

if [ -e 'crypto_md5_op_test' ]; then
    ./crypto_md5_op_test
fi

if [ -e 'encoding_base64_op_test' ]; then
    ./encoding_base64_op_test
fi

if [ -e 'redis_backend_test' ]; then
    ./redis_backend_test
fi
