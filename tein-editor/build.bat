@echo off

set defs=-DFC_USE_SDL_GPU -DTEINEDIT_WIN32
set flag=-O2 -EHsc
set idir=-I C:\Josh\Libraries\SDL2~2.0.6\include -I C:\Josh\Libraries\glew~1.13.0\include
set ldir=-libpath:C:\Josh\Libraries\SDL2~2.0.6\lib\x86 -libpath:C:\Josh\Libraries\glew~1.13.0\lib\Release\Win32
set libs=SDL2main.lib SDL2.lib SDL2_ttf.lib SDL2_gpu.lib glew32.lib opengl32.lib user32.lib kernel32.lib shell32.lib comdlg32.lib dbghelp.lib ole32.lib shlwapi.lib

if /I "%1%"=="d" goto build_debug
if /I "%1%"=="r" goto build_release
goto error

:build_debug
pushd bin
cl %idir% %defs% %flag% -Zi -Feeditor_debug.exe ..\source\main.cpp -link %ldir% %libs% -subsystem:console
popd bin
goto clean

:build_release
tools\datpak\datpak bin\data\*
del bin\data.pak
rc -fobin\icon.res resource\icon.rc
pushd bin
cl %idir% %defs% -DRELEASE %flag% -FeTEINEditor.exe ..\source\main.cpp -link %ldir% %libs% icon.res -subsystem:windows
popd bin
goto clean

:error
echo No such build mode!
goto end

:clean
del bin\*.ilk bin\*.res bin\*.obj bin\*.exp bin\*.lib

:end