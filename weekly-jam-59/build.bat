@echo off

set idir=-I ..\source\external
set ldir=-libpath:..\library\win32
set defs=-DTCE_PLATFORM_WIN32 -DSDL_MAIN_HANDLED -D_CRT_SECURE_NO_WARNINGS
set cflg=-nologo -Oi -MT -WX -W4 -Gm- -GR- -EHa-
set lflg=-opt:ref -incremental:no
set warn=-wd4100 -wd4505 -wd4189 -wd4244 -wd4577
set libs=SDL2main.lib SDL2.lib SDL2_mixer.lib lua53.lib kernel32.lib user32.lib advapi32.lib shlwapi.lib
set main=..\source\main.cpp
set name=engine.exe

if /I "%2%"=="r" (
	set lflg=%lflg% -subsystem:windows,5.1
) else (
	set defs=%defs% -DTCE_DEBUG
	set cflg=%cflg% -Z7
)

if not exist bin mkdir bin
pushd bin
cl %idir% %defs% %cflg% %warn% -Fe%name% %main% -link %lflg% %ldir% %libs%
del *.ilk *.res *.obj *.exp *.lib
popd
