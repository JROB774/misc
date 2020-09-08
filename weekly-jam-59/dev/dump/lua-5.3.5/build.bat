@echo off

if not exist bin-win32 mkdir bin-win32
pushd bin-win32
cl -MT -O2 -DLUA_BUILD_AS_DLL -c ..\src\*.c
del lua.obj
del luac.obj
link -dll -implib:lua53.lib -out:lua53.dll *.obj
del *.obj *.o
popd
