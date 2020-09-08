@echo off
echo Building project...
g++ -std=c++11 -Wall -Wzero-as-null-pointer-constant -pedantic -Os -s -o mappack mappack.cpp
echo Building complete!
pause