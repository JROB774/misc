@echo off
setlocal

pushd ..
call build\utility\buildvars.bat
pushd binary
%OutputExecutable%
popd
popd

endlocal
