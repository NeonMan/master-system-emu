#Savestate load/save
project(test_savestate)

add_executable (zzz_test_savestate test_savestate.c)
target_link_libraries (zzz_test_savestate unity savestate smsram smsrom smsio peripheral psg z80)
add_test(test_savestate zzz_test_savestate)
