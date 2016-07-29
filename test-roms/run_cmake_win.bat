@echo off

rem Make sure NMake is available on the PATH
set PATH=%PATH%;C:\Program Files (x86)\Microsoft Visual Studio 14.0\VC\bin

md Build
cd Build
cmake -DCMAKE_TOOLCHAIN_FILE=../Toolchain-sdcc.cmake -G "NMake Makefiles" ..
nmake clean
nmake

pause