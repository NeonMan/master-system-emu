This document describes the required tools and steps to build the emulator and
selftest ROMs.

# Master system emulator #

Emulator build is managed by CMake, required additional tools and tips below.

## Requisites ##

This project uses CMake to build the binaries. However, the following tools
must be available in your PATH. These tools should be readily available on any
linux distribution, windows versions are usually available on the links
provided.

* [CMake]
* [Python 3]
* FLUID ([FLTK] User Interface Designer)

Additionally, for coverage tests on a debug build (only on GCC) the following
tools should be available.

* gcov (part of gcc)
* [lcov]

## Building on Windows (Visual Studio) ##

Run the provided `run_cmake_win.bat` file, VS project files will be available
on the `build` directory.

## Building on GNU (Linux/MinGW/etc) ##

Point a shell into the source directory then run:

    cmake .
    make
    
Optionally, for a separate build tree, go to the source directory then run:

    mkdir build
    cd build
    cmake ..
    make

### Debug builds ###

To select debug build for coverage tests, add `-DCMAKE_BUILD_TYPE=Debug` to
the CMake command.

# Building test ROMs #

The test-roms directory is another CMake project that can be built separately. 

## Requisites ##

These tools must be installed and available on your PATH.

* [SDCC] (Small Device C Compiler)
* make (nmake on windows)
 
## Building on Windows ##

Run the provided `test-roms\run_cmake_win.bat` file, binaries should be readily
built and available on the `test-roms\Build` directory.

## Building on GNU ##

Building the test ROMs requires usign the provided toolchain file 
`Toolchain-sdcc.cmake` to use SDCC instead of gcc to build the ROM sources. 
Pointing a shell to the `test-roms` directory and executing the following shall
compile the ROM files in `.sms` and `.ihx` format

    cmake -DCMAKE_TOOLCHAIN_FILE=./Toolchain-sdcc.cmake .
    make

As with the emulator build, a separate build tree can be used.

   [lcov]:     http://ltp.sourceforge.net/coverage/lcov.php
   [CMake]:    https://cmake.org/
   [Python 3]: https://www.python.org
   [FLTK]:     http://www.fltk.org/index.php
   [SDCC]:     http://sdcc.sourceforge.net/
