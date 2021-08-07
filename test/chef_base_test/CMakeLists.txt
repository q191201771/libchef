add_executable(chef_buffer_test chef_buffer_test.cc)
target_link_libraries(chef_buffer_test ${COMMON_LIBS})

add_executable(chef_count_dump_test chef_count_dump_test.cc)
target_link_libraries(chef_count_dump_test ${COMMON_LIBS})

add_executable(chef_crypto_md5_op_test chef_crypto_md5_op_test.cc)
target_link_libraries(chef_crypto_md5_op_test ${COMMON_LIBS})

add_executable(chef_crypto_sha1_op_test chef_crypto_sha1_op_test.cc)
target_link_libraries(chef_crypto_sha1_op_test ${COMMON_LIBS})

add_executable(chef_defer_test chef_defer_test.cc)
target_link_libraries(chef_defer_test ${COMMON_LIBS})

add_executable(chef_encoding_base64_op_test chef_encoding_base64_op_test.cc)
target_link_libraries(chef_encoding_base64_op_test ${COMMON_LIBS})

add_executable(chef_filepath_op_test chef_filepath_op_test.cc)
target_link_libraries(chef_filepath_op_test ${COMMON_LIBS})

add_executable(chef_hash_test chef_hash_test.cc)
target_link_libraries(chef_hash_test ${COMMON_LIBS})

add_executable(chef_lru_test chef_lru_test.cc)
target_link_libraries(chef_lru_test ${COMMON_LIBS})

add_executable(chef_os_exec_op_test chef_os_exec_op_test.cc)
target_link_libraries(chef_os_exec_op_test ${COMMON_LIBS})

add_executable(chef_stringify_stl_test chef_stringify_stl_test.cc)
target_link_libraries(chef_stringify_stl_test ${COMMON_LIBS})

add_executable(chef_strings_op_test chef_strings_op_test.cc)
target_link_libraries(chef_strings_op_test ${COMMON_LIBS})

add_executable(chef_task_thread_test chef_task_thread_test.cc)
target_link_libraries(chef_task_thread_test ${COMMON_LIBS})

add_executable(chef_thread_group_test chef_thread_group_test.cc)
target_link_libraries(chef_thread_group_test ${COMMON_LIBS})

add_executable(chef_thread_pool_test chef_thread_pool_test.cc)
target_link_libraries(chef_thread_pool_test ${COMMON_LIBS})

add_executable(chef_wait_event_test chef_wait_event_test.cc)
target_link_libraries(chef_wait_event_test ${COMMON_LIBS})

add_executable(chef_wait_event_counter_test chef_wait_event_counter_test.cc)
target_link_libraries(chef_wait_event_counter_test ${COMMON_LIBS})

add_executable(chef_fmt_op_test chef_fmt_op_test.cc)
target_link_libraries(chef_fmt_op_test ${COMMON_LIBS})

add_executable(chef_env_var_op_test chef_env_var_op_test.cc)
target_link_libraries(chef_env_var_op_test ${COMMON_LIBS})

add_executable(chef_this_machine_op_test chef_this_machine_op_test.cc)
target_link_libraries(chef_this_machine_op_test ${COMMON_LIBS})

add_executable(chef_this_process_op_test chef_this_process_op_test.cc)
target_link_libraries(chef_this_process_op_test ${COMMON_LIBS})

add_executable(chef_weight_selector_test chef_weight_selector_test.cc)
target_link_libraries(chef_weight_selector_test ${COMMON_LIBS})

add_executable(chef_snippet_test chef_snippet_test.cc)
target_link_libraries(chef_snippet_test ${COMMON_LIBS})

add_executable(chef_consistent_hash_test chef_consistent_hash_test.cc)
target_link_libraries(chef_consistent_hash_test ${COMMON_LIBS})

add_executable(chef_skiplist_test chef_skiplist_test.cc)
target_link_libraries(chef_skiplist_test ${COMMON_LIBS})

add_executable(chef_daemon_op_test chef_daemon_op_test.cc)
target_link_libraries(chef_daemon_op_test ${COMMON_LIBS})

add_executable(chef_stuff_op_test chef_stuff_op_test.cc)
target_link_libraries(chef_stuff_op_test ${COMMON_LIBS})

add_executable(chef_filelock_test chef_filelock_test.cc)
target_link_libraries(chef_filelock_test ${COMMON_LIBS})

add_executable(chef_crypto_sha256_op_test chef_crypto_sha256_op_test.cc)
target_link_libraries(chef_crypto_sha256_op_test ${COMMON_LIBS})

add_executable(chef_crypto_hmac_sha256_test chef_crypto_hmac_sha256_test.cc)
target_link_libraries(chef_crypto_hmac_sha256_test ${COMMON_LIBS})

add_executable(chef_be_le_op_test chef_be_le_op_test.cc)
target_link_libraries(chef_be_le_op_test ${COMMON_LIBS})

add_executable(chef_debug_mutex_test chef_debug_mutex_test.cc)
target_link_libraries(chef_debug_mutex_test ${COMMON_LIBS})
