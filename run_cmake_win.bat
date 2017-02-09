@echo off

:start
set choice=E

md build
cd build
cmake ..
cd ..

echo 
echo (R) Rebuild
echo (E) Exit

set /p choice=Option: 
if '%choice%'=='R' goto start
if '%choice%'=='r' goto start
