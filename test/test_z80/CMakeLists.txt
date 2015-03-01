#Unity testing framework
project(unity)
add_executable(test_z80_decoder test_z80_decoder.c)
target_link_libraries(test_z80_decoder z80 unity)
