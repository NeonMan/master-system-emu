#Test
project(test)

include_directories(unity)
include_directories(${PROJECT_SOURCE_DIR})

add_subdirectory (unity)
add_subdirectory (test_z80)
add_subdirectory (test_z80_undoc)
add_subdirectory (test_mem)
add_subdirectory (test_savestate)
