#!/bin/bash

idir=-I ../source/external
ldir=-L ../library/linux
defs=-DTCE_PLATFORM_LINUX
cflg=-std=c++17 -Wall -Werror -pedantic -fno-exceptions
lflg=
warn=-Wconversion -Wno-unused-variable -Wno-unused-function -Wno-unused-but-set-variable
libs=-lmingw32 -static-libgcc -static-libstdc++ -lSDL2main -SDL2 -lSDL2_mixer -llua53
main=../source/main.cpp
name=engine.exe

shopt -s nocasematch
if [[%1 == "r"]]
then
	cflg=%cflg -s
	lflg=%lflg -s -Wl,--subsystem,windows
else
	defs=%defs -DTCE_DEBUG
	cflg=%cflg -g
fi

mkdir -p bin
pushd bin
gcc %idir %defs %cflg %warn -o %name %main %ldir %lflg %libs
popd
