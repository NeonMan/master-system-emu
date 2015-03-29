project(test_z80)

add_executable(test_z80_decoder test_z80_decoder.c)
target_link_libraries(test_z80_decoder z80 unity)
add_test(_test_z80_decoder test_z80_decoder)

add_executable(test_z80_macros test_z80_macros.c)
target_link_libraries(test_z80_macros z80 unity)
add_test(_test_z80_macros test_z80_macros)
