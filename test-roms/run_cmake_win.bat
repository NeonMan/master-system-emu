@echo off

rem Make sure NMake is available on the PATH
set PATH=%PATH%;C:\Program Files (x86)\Microsoft Visual Studio 14.0\VC\bin

:start
set choice=E

md Build
cd Build
cmake -DCMAKE_TOOLCHAIN_FILE=../Toolchain-sdcc.cmake -G "NMake Makefiles" ..
nmake clean
nmake
cd ..

echo.
echo (R) Rebuild
echo (E) Exit

set /p choice=Option: 
if '%choice%'=='R' goto start
if '%choice%'=='r' goto start
