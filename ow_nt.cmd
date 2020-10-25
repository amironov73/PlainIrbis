@echo off

rem ====================================================
rem Build for Win32 with Open Watcom 
rem ====================================================

mkdir build     > nul 2> nul
mkdir artifacts > nul 2> nul
cd build

cmake .. -G "Watcom WMake" -D CMAKE_BUILD_TYPE=Release -D WITH_TESTS=OFF
cmake --build .
cmake --build . --target install

cd ..