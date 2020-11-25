@echo off

set Libraries=setargv.obj

set Architecture=amd64
set BuildMode=Debug

set CompilerFlags=-nologo -W4 -MT -EHsc -Oi -Z7
set LinkerFlags=-opt:ref -incremental:no -subsystem:console

set CompilerWarnings=-wd4100 -wd4505 -wd4189
set LinkerWarnings=-ignore:4099

set Defines=

set InputSource=..\source\main.cpp

set OutputExecutable=threads
