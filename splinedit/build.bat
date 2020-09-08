@echo off
setlocal

set idir=-I ..\source\external
set ldir=-libpath:..\library
set defs=-D_CRT_SECURE_NO_WARNINGS -DSDL_MAIN_HANDLED
set cflg=-nologo -Oi -MT -WX -W4 -Gm- -GR- -EHa-
set lflg=-opt:ref -incremental:no
set warn=-wd4100 -wd4505 -wd4189
set libs=SDL2main.lib SDL2.lib setargv.obj
set main=..\source\main.cpp
set name=Splinedit.exe

call "C:\Program Files (x86)\Microsoft Visual Studio\2017\Community\Common7\Tools\vsdevcmd.bat" -arch=x86

if not exist bin mkdir bin
pushd bin
cl %idir% %defs% %cflg% %warn% -Fe%name% %main% -link %lflg% %ldir% %libs%
del *.ilk *.res *.obj *.exp *.lib
popd

endlocal
