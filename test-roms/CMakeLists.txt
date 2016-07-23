cmake_minimum_required (VERSION 2.8)
project(test-roms C)
ENABLE_LANGUAGE(C)

#Configure SDCC to make z80 binaries
set(CMAKE_C_FLAGS "-mz80 --std-c99 --data-loc 0xc000 --no-std-crt0")

#crt0 for Sega Master System
# -- ToDo. Using a prebuilt one

#Libraries
ADD_LIBRARY(sdsc sdsc.c)
ADD_LIBRARY(emu-hooks emu-hooks.c)

#ROM "executables" ;)
ADD_EXECUTABLE(sdcc-sample sdcc-sample.c)
TARGET_LINK_LIBRARIES(sdcc-sample sdsc emu-hooks )
