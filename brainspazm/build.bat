@echo off

set idir=
set ldir=
set defs=
set flag=-std=c++11 -pedantic -O3
set warn=-Wall -Werror -Wno-unused-variable -Wno-unused-function -Wno-unused-but-set-variable
set libs=-s -static-libgcc -static-libstdc++ -static -lpthread
set main=main.cpp
set name=Brainspazm.exe

windres icon.rc -O coff -o icon.res
g++ %idir% %defs% %flag% %warn% -c -o main.o %main%
g++ -o %name% main.o icon.res %ldir% %libs%

del main.o, icon.res