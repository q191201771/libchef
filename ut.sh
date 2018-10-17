rm -rf output && \
scons -c && \
scons -j8 mode=debug && \
echo '----------------------------------------' && \
./output/chef_base_test/chef_buffer_test && \
./output/chef_base_test/chef_count_dump_test && \
./output/chef_base_test/chef_crypto_md5_op_test && \
./output/chef_base_test/chef_crypto_sha1_op_test && \
./output/chef_base_test/chef_defer_test && \
./output/chef_base_test/chef_encoding_base64_op_test && \
./output/chef_base_test/chef_env_var_op_test && \
./output/chef_base_test/chef_filepath_op_test && \
./output/chef_base_test/chef_fmt_op_test && \
./output/chef_base_test/chef_hash_test && \
./output/chef_base_test/chef_lru_test && \
./output/chef_base_test/chef_os_exec_op_test && \
./output/chef_base_test/chef_snippet_test && \
./output/chef_base_test/chef_stringify_stl_test && \
./output/chef_base_test/chef_strings_op_test && \
./output/chef_base_test/chef_task_thread_test && \
./output/chef_base_test/chef_this_machine_op_test && \
./output/chef_base_test/chef_this_process_op_test && \
./output/chef_base_test/chef_thread_group_test && \
./output/chef_base_test/chef_thread_pool_test && \
./output/chef_base_test/chef_wait_event_test && \
./output/chef_base_test/chef_wait_event_counter_test && \
./output/chef_base_test/chef_weight_selector_test && \
rm -rf chef_thread_group_test.dump && \
rm -rf not_exist && \
echo '----------------------------------------' && \
echo 'all unit test succ. :)'

#./output/base_test/chef_http_op_test
#./output/base_test/chef_log_test

