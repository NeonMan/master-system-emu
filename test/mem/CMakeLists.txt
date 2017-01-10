#RAM/ROM testing
project(test_mem)

add_executable (zzz_test_mem test_mem.c)
target_link_libraries (zzz_test_mem smsrom smsio glue sms_debug smsram)
add_test(test_mem zzz_test_mem)
