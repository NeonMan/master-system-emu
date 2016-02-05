#Savestate load/save
project(test_savestate)

add_executable (zzz_test_savestate test_savestate.c)
target_link_libraries (zzz_test_savestate unity smsrom smsio smsram z80 psg vdp peripheral savestate)
add_test(test_savestate zzz_test_savestate)
