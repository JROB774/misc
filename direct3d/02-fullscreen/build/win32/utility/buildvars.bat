:: Build Configuration Variables Script

@echo off

set Libraries=kernel32.lib user32.lib d3d11.lib setargv.obj

:: Possible Values: x86, amd64
set Architecture=amd64

:: Possible Values: Debug, Release
set BuildMode=Debug

set IncludeDirs=
set LibraryDirs=

set Defines=

set CompilerFlags=-Zc:__cplusplus -std:c++17 -nologo -W4 -MT -Oi -GR- -EHsc -Z7
set LinkerFlags=-opt:ref -incremental:no -subsystem:windows

set CompilerWarnings=-wd4100 -wd4505 -wd4189
set LinkerWarnings=-ignore:4099

set ResourceFile=
set ResourcePath=

set InputResource=
set InputSource=..\source\main.cpp

set OutputExecutable=LearnDirect3D11-%Architecture%

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
