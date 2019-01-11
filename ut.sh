#/bin/bash

./clean.sh && \
./build.sh
echo '----------------------------------------' && \
./output/bin/chef_buffer_test && \
./output/bin/chef_consistent_hash_test && \
./output/bin/chef_count_dump_test && \
./output/bin/chef_crypto_md5_op_test && \
./output/bin/chef_crypto_sha1_op_test && \
./output/bin/chef_defer_test && \
./output/bin/chef_encoding_base64_op_test && \
./output/bin/chef_env_var_op_test && \
./output/bin/chef_filepath_op_test && \
./output/bin/chef_fmt_op_test && \
./output/bin/chef_hash_test && \
./output/bin/chef_lru_test && \
./output/bin/chef_os_exec_op_test && \
./output/bin/chef_skiplist_test && \
./output/bin/chef_snippet_test && \
./output/bin/chef_stringify_stl_test && \
./output/bin/chef_strings_op_test && \
./output/bin/chef_task_thread_test && \
./output/bin/chef_this_machine_op_test && \
./output/bin/chef_this_process_op_test && \
./output/bin/chef_thread_group_test && \
./output/bin/chef_thread_pool_test && \
./output/bin/chef_wait_event_test && \
./output/bin/chef_wait_event_counter_test && \
./output/bin/chef_weight_selector_test && \
./output/bin/chef_daemon_op_test && \
./output/bin/chef_stuff_op_test && \
./output/bin/chef_filelock_test && \
./output/bin/chef_crypto_sha256_op_test && \
./output/bin/chef_crypto_hmac_sha256_test && \
rm -rf chef_thread_group_test.dump && \
rm -rf not_exist && \
echo '----------------------------------------' && \
echo 'all unit test succ. :)'

