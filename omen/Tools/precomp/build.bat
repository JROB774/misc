@echo off
echo Building project...
g++ -std=c++11 -Wall -Wzero-as-null-pointer-constant -pedantic -Os -s -o precomp precomp.cpp
echo Building complete!
pause