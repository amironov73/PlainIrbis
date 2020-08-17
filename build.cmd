@echo off

mkdir build     > nul 2> nul
mkdir artifacts > nul 2> nul

set CMAKE="cmake"

%CMAKE% -B ./build -D CMAKE_BUILD_TYPE=Release .
%CMAKE% --build ./build --config Release -j 4
%CMAKE% --install ./build --config Release 
