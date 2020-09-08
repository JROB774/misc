@echo off

set idir=
set ldir=
set defs=
set flag=-std=c++11 -pedantic -O3
set warn=-Wall -Werror -Wno-unused-variable -Wno-unused-function -Wno-unused-but-set-variable
set libs=-s -static-libgcc -static-libstdc++ -luser32 -lcomdlg32 -lcomctl32 -lgdi32 -lgdiplus -mwindows -static -lpthread
set main=main.cpp
set name=ImagePreview.exe

windres icon.rc -O coff -o icon.res
windres dialog.rc -O coff -o dialog.res
windres menu.rc -O coff -o menu.res

g++ %idir% %defs% %flag% %warn% -c -o main.o %main%
g++ -o %name% main.o icon.res dialog.res menu.res %ldir% %libs%

del main.o, icon.res, dialog.res, menu.res