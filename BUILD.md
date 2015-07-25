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

## Debug builds ##

To select debug build for coverage tests, add `-DCMAKE_BUILD_TYPE=Debug` to the
CMake command.