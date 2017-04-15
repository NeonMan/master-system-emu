#VDP internals
project(test_vdp)

add_executable (zzz_test_vdp_rw test_vdp_rw.c)
target_link_libraries (zzz_test_vdp_rw unity vdp smsram smsrom smsio peripheral psg z80 glue sms_debug)
add_test(test_vdp_rw zzz_test_vdp_rw)
