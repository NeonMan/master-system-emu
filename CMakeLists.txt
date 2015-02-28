# Main
cmake_minimum_required (VERSION 2.8.11)

# --- Project name ---
project (SMS-Emu)

# --- Explicitly enable C
enable_language (C)
enable_language (CXX)

# --- Enable C99 on GCC
if (CMAKE_COMPILER_IS_GNUCC)
    SET(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wall -Werror -std=c99")
endif (CMAKE_COMPILER_IS_GNUCC)

# --- Enable C++11 on G++
if (CMAKE_COMPILER_IS_GNUCXX)
    SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Werror -std=c++11")
endif (CMAKE_COMPILER_IS_GNUCXX)

# --- MSVC configs
if (MSVC)
    #Tell MSVC to shut the fuck up about _CRT_SECURE_NO_WARNINGS.
    add_definitions( -D_CRT_SECURE_NO_WARNINGS )
    #Make it a little more verbose and set warnings-as-errors.
    SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /W3 /WX")
    SET(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} /W3 /WX")
endif (MSVC)

# --- Enable OpenMP if available
find_package(OpenMP)
if (OPENMP_FOUND)
    set (CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${OpenMP_C_FLAGS}")
    set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${OpenMP_CXX_FLAGS}")
endif(OPENMP_FOUND)

# --- Dependencies ---
include_directories(${PROJECT_SOURCE_DIR})
add_subdirectory (glue)
add_subdirectory (psg)
add_subdirectory (ram)
add_subdirectory (rom)
add_subdirectory (z80)
add_subdirectory (peripherial)
add_subdirectory (io)
add_subdirectory (vdp)
add_subdirectory (sdsc)
add_subdirectory (sms-ui)
add_subdirectory (test)
#z80 tests not CMake-ready yet
#add_subdirectory (z80tests)

# --- The project executables ---

# --- psg-play
add_executable (psg-play psg-play.c)
target_link_libraries (psg-play psg SDL2 SDL2main)

# --- test-mem
add_executable (test-mem test-mem.c)
target_link_libraries (test-mem smsrom smsio smsram)

# --- test-z80
add_executable (test-z80 test-z80.c)
target_link_libraries (test-z80 z80 smsrom smsram sdsc)

# --- test-vdp (SDL2 is presumed to be available)
add_executable (test-vdp test-vdp.c)
target_link_libraries (test-vdp vdp SDL2 SDL2main)

# --- sms-emu
add_executable (emu sms-emu.c)
target_link_libraries (emu glue smsrom smsio smsram psg z80 peripherial vdp sdsc SDL2 SDL2main)
