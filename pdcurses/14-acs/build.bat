@echo off

set cflg=-nologo -Oi -MT -Gm- -GR- -EHa- -O2
set defs=
set idir=-I .. -I C:\Josh\Libraries\pdcurses~3.6\include
set ldir=-libpath:C:\Josh\Libraries\pdcurses~3.6\lib\msvc-x86-static
set lflg=-opt:ref -incremental:no
set libs=pdcurses.lib user32.lib advapi32.lib setargv.obj
set name=test.exe

cl %cflg% %defs% %idir% -Fe%name% main.cpp -link %ldir% %lflg% %libs%
del *.ilk *.res *.obj
