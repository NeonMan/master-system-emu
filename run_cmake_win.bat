@echo off
rem This batch file replaces the hardcoded CMakeLists lines
rem used in the qt-requiring projects
rem
rem #Kit: MSVC 2013 kit
rem set(CMAKE_PREFIX_PATH "C:\\Qt\\5.3\\msvc2013")
rem #Kit: MSVC 2013 OpenGL
rem #set(CMAKE_PREFIX_PATH "C:\\Qt\\5.3\\msvc2013_opengl")
rem #Kit: Mingw 482
rem #set(CMAKE_PREFIX_PATH "C:\\Qt\\5.3\\mingw482_32")

rem MSVC 2013
set CMAKE_PREFIX_PATH=C:\Qt\5.3\msvc2013

rem MSVC 2013 OpenGL
rem set CMAKE_PREFIX_PATH=C:\Qt\5.3\msvc2013_opengl

rem Mingw 482
rem set CMAKE_PREFIX_PATH=C:\Qt\5.3\mingw482_32

md build
cd build
cmake ..

pause