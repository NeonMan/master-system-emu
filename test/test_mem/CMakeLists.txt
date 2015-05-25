#Unity testing framework
project(test_mem)

add_executable (zzz_test_mem test_mem.c)
target_link_libraries (zzz_test_mem smsrom smsio smsram)
add_test(test_mem zzz_test_mem)
