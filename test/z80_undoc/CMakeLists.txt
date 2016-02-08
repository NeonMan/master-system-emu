project(test_z80_undoc)

#8-bit LD
add_executable(zzz_test_z80_ld8_undoc test_z80_ld8_undoc.c)
target_link_libraries(zzz_test_z80_ld8_undoc z80 smsram unity)
add_test(test_z80_ld8_undoc zzz_test_z80_ld8_undoc)
