@echo off

:start
set choice=E

md build64
cd build64
cmake -G "Visual Studio 14 2015 Win64" ..
cd ..

echo 
echo (R) Rebuild
echo (E) Exit

set /p choice=Option: 
if '%choice%'=='R' goto start
if '%choice%'=='r' goto start
