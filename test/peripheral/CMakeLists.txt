#Periphaeral testing
project(test_peripheral)

add_executable (zzz_test_peripheral test_peripheral.c)
target_link_libraries (zzz_test_peripheral unity peripheral z80)
add_test(test_peripheral zzz_test_peripheral)
