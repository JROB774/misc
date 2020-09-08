#!/bin/bash

gcc -std=c89 -O2 -DLUA_BUILD_AS_DLL -c -fPIC ./src/lapi.c
gcc -std=c89 -O2 -DLUA_BUILD_AS_DLL -c -fPIC ./src/lauxlib.c
gcc -std=c89 -O2 -DLUA_BUILD_AS_DLL -c -fPIC ./src/lbaselib.c
gcc -std=c89 -O2 -DLUA_BUILD_AS_DLL -c -fPIC ./src/lbitlib.c
gcc -std=c89 -O2 -DLUA_BUILD_AS_DLL -c -fPIC ./src/lcode.c
gcc -std=c89 -O2 -DLUA_BUILD_AS_DLL -c -fPIC ./src/lcorolib.c
gcc -std=c89 -O2 -DLUA_BUILD_AS_DLL -c -fPIC ./src/lctype.c
gcc -std=c89 -O2 -DLUA_BUILD_AS_DLL -c -fPIC ./src/ldblib.c
gcc -std=c89 -O2 -DLUA_BUILD_AS_DLL -c -fPIC ./src/ldebug.c
gcc -std=c89 -O2 -DLUA_BUILD_AS_DLL -c -fPIC ./src/ldo.c
gcc -std=c89 -O2 -DLUA_BUILD_AS_DLL -c -fPIC ./src/ldump.c
gcc -std=c89 -O2 -DLUA_BUILD_AS_DLL -c -fPIC ./src/lfunc.c
gcc -std=c89 -O2 -DLUA_BUILD_AS_DLL -c -fPIC ./src/lgc.c
gcc -std=c89 -O2 -DLUA_BUILD_AS_DLL -c -fPIC ./src/linit.c
gcc -std=c89 -O2 -DLUA_BUILD_AS_DLL -c -fPIC ./src/liolib.c
gcc -std=c89 -O2 -DLUA_BUILD_AS_DLL -c -fPIC ./src/llex.c
gcc -std=c89 -O2 -DLUA_BUILD_AS_DLL -c -fPIC ./src/lmathlib.c
gcc -std=c89 -O2 -DLUA_BUILD_AS_DLL -c -fPIC ./src/lmem.c
gcc -std=c89 -O2 -DLUA_BUILD_AS_DLL -c -fPIC ./src/loadlib.c
gcc -std=c89 -O2 -DLUA_BUILD_AS_DLL -c -fPIC ./src/lobject.c
gcc -std=c89 -O2 -DLUA_BUILD_AS_DLL -c -fPIC ./src/lopcodes.c
gcc -std=c89 -O2 -DLUA_BUILD_AS_DLL -c -fPIC ./src/loslib.c
gcc -std=c89 -O2 -DLUA_BUILD_AS_DLL -c -fPIC ./src/lparser.c
gcc -std=c89 -O2 -DLUA_BUILD_AS_DLL -c -fPIC ./src/lstate.c
gcc -std=c89 -O2 -DLUA_BUILD_AS_DLL -c -fPIC ./src/lstring.c
gcc -std=c89 -O2 -DLUA_BUILD_AS_DLL -c -fPIC ./src/lstrlib.c
gcc -std=c89 -O2 -DLUA_BUILD_AS_DLL -c -fPIC ./src/ltable.c
gcc -std=c89 -O2 -DLUA_BUILD_AS_DLL -c -fPIC ./src/ltablib.c
gcc -std=c89 -O2 -DLUA_BUILD_AS_DLL -c -fPIC ./src/ltm.c
gcc -std=c89 -O2 -DLUA_BUILD_AS_DLL -c -fPIC ./src/lua.c
gcc -std=c89 -O2 -DLUA_BUILD_AS_DLL -c -fPIC ./src/luac.c
gcc -std=c89 -O2 -DLUA_BUILD_AS_DLL -c -fPIC ./src/lundump.c
gcc -std=c89 -O2 -DLUA_BUILD_AS_DLL -c -fPIC ./src/lutf8lib.c
gcc -std=c89 -O2 -DLUA_BUILD_AS_DLL -c -fPIC ./src/lvm.c
gcc -std=c89 -O2 -DLUA_BUILD_AS_DLL -c -fPIC ./src/lzio.c
rm lua.o
rm luac.o
gcc -shared -o liblua53.so *.o -lmingw32 -static-libgcc
rm *.o
