@echo off
setlocal

pushd ..
call build\utility\findvsdev.bat
call build\utility\buildvars.bat
call %VSDevPath% -no_logo -arch=%Architecture%
devenv binary\%OutputExecutable%.exe
popd

endlocal
