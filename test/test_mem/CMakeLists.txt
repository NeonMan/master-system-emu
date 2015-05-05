#Unity testing framework
project(test_mem)

add_executable (test_mem test_mem.c)
target_link_libraries (test_mem smsrom smsio smsram)
add_test(_test_mem test_mem)
