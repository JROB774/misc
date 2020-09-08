@echo off

set idir=-isystem "..\source\external"
set ldir=-L "..\library\win32"
set defs=-DTCE_PLATFORM_WIN32
set cflg=-std=c++11 -Wall -Werror -pedantic -fno-exceptions
set lflg=
set warn=
set libs=-lmingw32 -static-libgcc -static-libstdc++ -lSDL2main -lSDL2 -lSDL2_mixer -llua53 -lkernel32 -luser32
set main=..\source\main.cpp
set name=engine.exe

if not exist bin mkdir bin
pushd bin
g++ %idir% %defs% %cflg% %warn% -o %name% %main% %ldir% %libs%
popd
