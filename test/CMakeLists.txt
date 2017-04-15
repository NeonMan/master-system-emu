#Test
project(test)

include_directories(unity)
include_directories(${PROJECT_SOURCE_DIR})

add_subdirectory (unity)
add_subdirectory (z80)
add_subdirectory (z80_undoc)
add_subdirectory (mem)
add_subdirectory (savestate)
add_subdirectory (peripheral)
add_subdirectory (vdp)
