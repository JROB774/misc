@echo off

if /I "%1%"=="d" goto run_debug
if /I "%1%"=="r" goto run_release
goto error

:run_debug
pushd bin
editor_debug.exe
popd bin
goto end

:run_release
pushd bin
TEINEditor.exe
popd bin
goto end

:error
echo No such run mode!

:end