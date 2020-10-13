@echo off
setlocal

call "C:\Program Files (x86)\Microsoft Visual Studio\2017\Community\Common7\Tools\vsdevcmd.bat" -arch=x86

set idir=-I C:\Josh\Libraries\pcre2~10.31\VC\x86\include
set defs=-D_CRT_SECURE_NO_WARNINGS -DWIN32_LEAN_AND_MEAN -DPCRE2_STATIC -DPCRE2_CODE_UNIT_WIDTH=8
set cflg=-nologo -Oi -MT -WX -W4 -Gm- -GR- -EHa-
set lflg=-opt:ref -incremental:no
set warn=-wd4100 -wd4505 -wd4189
set ldir=-libpath:C:\Josh\Libraries\pcre2~10.31\VC\x86\lib
set libs=pcre2-8s.lib
set main=fsearch.cpp
set name=fsearch.exe

cl %idir% %defs% %cflg% %warn% -Fe%name% %main% -link %lflg% %ldir% %libs%
del *.ilk *.res *.obj *.exp *.lib

endlocal
