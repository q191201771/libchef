#!/bin/bash

cd build/base_test
./os_exec_op_test
./crypto_md5_test
./log_test
./encoding_base64_test
./json_config_test
#./daemon_test
./chunk_head_op_test
./filepath_op_test
./noncopyable_test
./wait_event_test
./this_thread_op_test
./task_thread_test
#./redis_backend_test
./thread_pool_test