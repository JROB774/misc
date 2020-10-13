@echo off

set idir=
set ldir=
set defs=
set flag=-std=c99 -pedantic -O3
set warn=-Wall -Werror -Wno-unused-variable -Wno-unused-function -Wno-unused-but-set-variable 
set libs=-s -static-libgcc
set main=linecount.c
set name=linecount.exe

gcc %idir% %defs% %flag% %warn% -o %name% %main% %ldir% %libs%