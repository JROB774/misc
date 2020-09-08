@echo off
setlocal

call "C:\Program Files (x86)\Microsoft Visual Studio\2017\Community\Common7\Tools\vsdevcmd.bat" -arch=amd64
start devenv bin\engine.exe

endlocal
