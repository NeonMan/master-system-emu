project(test_z80)

add_executable(zzz_test_z80_decoder test_z80_decoder.c)
target_link_libraries(zzz_test_z80_decoder z80 unity)
add_test(test_z80_decoder zzz_test_z80_decoder)

add_executable(zzz_test_z80_macros test_z80_macros.c)
target_link_libraries(zzz_test_z80_macros z80 unity)
add_test(test_z80_macros zzz_test_z80_macros)

add_executable(zzz_test_z80_stack test_z80_stack.c)
target_link_libraries(zzz_test_z80_stack z80 smsram unity)
add_test(test_z80_stack zzz_test_z80_stack)
