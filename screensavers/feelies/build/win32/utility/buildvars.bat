:: Build Configuration Variables Script

@echo off

set Libraries=kernel32.lib user32.lib gdi32.lib winmm.lib opengl32.lib setargv.obj

:: Select which architecture you want by uncommenting.
:: set Architecture=x86
   set Architecture=amd64

:: Select which build mode you want by uncommenting.
   set BuildMode=Debug
:: set BuildMode=Release

set IncludeDirs=-I ..\source\ -I ..\..\.screensaver\ -I ..\..\.screensaver\standard\
set LibraryDirs=

set Defines=

set CompilerFlags=-Zc:__cplusplus -std:c++17 -nologo -W4 -Oi -Gm- -GR- -EHa- -GS- -Gs9999999 -Z7
set LinkerFlags=-opt:ref -incremental:no -nodefaultlib -stack:0x100000,0x100000 -subsystem:windows

set CompilerWarnings=-wd4100 -wd4505 -wd4189
set LinkerWarnings=-ignore:4099

set ResourceFile=..\resource\resources.rc
set ResourcePath=..\resource\

set InputResource=..\resource\resources.res
set InputSource=..\..\.screensaver\main_win32.cpp

set OutputExecutable=Feelies

if %BuildMode%==Release (
    set CompilerFlags=%CompilerFlags% -WX
)
if %BuildMode%==Debug (
    set OutputExecutable=%OutputExecutable%-Debug
    set Defines=%Defines% -DBUILD_DEBUG
    set InputResource=
)

if %Architecture%==x86 (
    set CompilerFlags=%CompilerFlags% -arch:IA32
    set LinkerFlags=%LinkerFlags%,5.1
)
