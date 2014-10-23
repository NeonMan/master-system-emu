# Main
cmake_minimum_required (VERSION 2.8)

# --- Uncomment to use clang on linux
#if (UNIX)
#    SET (CMAKE_C_COMPILER "/usr/bin/clang")
#    SET (CMAKE_CXX_COMPILER "/usr/bin/clang++")
#endif (UNIX)

# --- Explicitly enable C
enable_language (C)

# --- Enable C99 on GCC
if (CMAKE_COMPILER_IS_GNUCC)
    SET(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -std=c99")
endif (CMAKE_COMPILER_IS_GNUCC)

# --- Tell MSVC to shut the fuck up about _CRT_SECURE_NO_WARNINGS
if (MSVC)
    add_definitions( -D_CRT_SECURE_NO_WARNINGS )
endif (MSVC)

# --- Enable OpenMP if available
find_package(OpenMP)
if (OPENMP_FOUND)
    set (CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${OpenMP_C_FLAGS}")
    set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${OpenMP_CXX_FLAGS}")
endif(OPENMP_FOUND)

# --- Dependencies ---
# --- PSG
include_directories ("${PROJECT_SOURCE_DIR}/psg")
add_subdirectory (psg)
# --- RAM
include_directories ("${PROJECT_SOURCE_DIR}/ram")
add_subdirectory (ram)
# --- ROM
include_directories ("${PROJECT_SOURCE_DIR}/rom")
add_subdirectory (rom)
# --- z80
include_directories ("${PROJECT_SOURCE_DIR}/z80")
add_subdirectory (z80)
# --- Peripherial
include_directories ("${PROJECT_SOURCE_DIR}/peripherial")
add_subdirectory (peripherial)
# --- IO
include_directories ("${PROJECT_SOURCE_DIR}/io")
add_subdirectory (io)
# --- VDP
include_directories ("${PROJECT_SOURCE_DIR}/vdp")
add_subdirectory (vdp)
# --- SDSC console
include_directories ("${PROJECT_SOURCE_DIR}/sdsc")
add_subdirectory (sdsc)


# Z80 assembler tests are only compiled on unix
if (UNIX)
  # --- Z80Tests
  add_subdirectory (z80tests)
endif (UNIX)

# --- The project executables ---
project (SMS-Emu)
# --- psg-play
add_executable (psg-play psg-play.c)
target_link_libraries (psg-play psg)
# --- test-mem
add_executable (test-mem test-mem.c)
target_link_libraries (test-mem rom io ram)
# --- test-z80
add_executable (test-z80 test-z80.c)
target_link_libraries (test-z80 z80 rom ram sdsc)
