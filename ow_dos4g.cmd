@echo off

rem ====================================================
rem Build for DOS4GW extender with Open Watcom 
rem ====================================================

mkdir build     > nul 2> nul
mkdir artifacts > nul 2> nul
cd build

cmake .. -G "Watcom WMake" -D CMAKE_BUILD_TYPE=Release -D WITH_TESTS=OFF -D CMAKE_C_FLAGS=-bt=dos4g
cmake --build .
cmake --build . --target install

cd ..