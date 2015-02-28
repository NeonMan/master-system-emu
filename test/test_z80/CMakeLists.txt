#Unity testing framework
project(unity)
add_executable(test_z80 test_z80.c)
target_link_libraries(test_z80 z80 unity)
