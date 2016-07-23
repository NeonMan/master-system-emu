# Building #

## Requisites ##

This project uses CMake to help building the binaries. However, the following
tools must be available in your path for a successful build.

* CMake
* Python 3
* FLUID (FLTK User Interface Designer)

Additionally, for coverage tests on a debug build (only on GCC) the following
tools should be available.

* gcov
* lcov

## Building ##

### Windows ###

Run the provided `run_cmake_win.bat` file, VS project files will be available
on the `build` directory

### GNU (Linux/MinGW/etc) ###

Point a shell into the source directory then run:

    cmake .
    make
    
Optionally, for a separate build tree, go to the source directory then run:

    mkdir build
    cd build
    cmake ..
    make

### Cross compile on linux MinGW ###

A toolchain file is provided to cross-build using MinGW on linux. Instructions
are the same as with regular GNU but providing CMake with the toolchain file:

    cmake -DCMAKE_TOOLCHAIN_FILE=../Toolchain-cross-mingw32-linux.cmake ..

### Debug builds ###

To select debug build for coverage tests, add `-DCMAKE_BUILD_TYPE=Debug` to the
CMake command.

# Building test ROMs #

The test-roms directory has another CMake project that can be built separately. 

## Requisites ##

* SDCC (Small Device C Compiler)
* make (nmake on windows works as well)
