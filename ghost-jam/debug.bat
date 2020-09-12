@echo off
setlocal

set vs_ent19="C:\Program Files (x86)\Microsoft Visual Studio\2019\Enterprise\Common7\Tools\vsdevcmd.bat"
set vs_pro19="C:\Program Files (x86)\Microsoft Visual Studio\2019\Professional\Common7\Tools\vsdevcmd.bat"
set vs_com19="C:\Program Files (x86)\Microsoft Visual Studio\2019\Community\Common7\Tools\vsdevcmd.bat"
set vs_ent17="C:\Program Files (x86)\Microsoft Visual Studio\2017\Enterprise\Common7\Tools\vsdevcmd.bat"
set vs_pro17="C:\Program Files (x86)\Microsoft Visual Studio\2017\Professional\Common7\Tools\vsdevcmd.bat"
set vs_com17="C:\Program Files (x86)\Microsoft Visual Studio\2017\Community\Common7\Tools\vsdevcmd.bat"

if exist %vs_ent19% ( call %vs_ent19% -no_logo -arch=amd64 )
if exist %vs_pro19% ( call %vs_pro19% -no_logo -arch=amd64 )
if exist %vs_com19% ( call %vs_com19% -no_logo -arch=amd64 )
if exist %vs_ent17% ( call %vs_ent17% -no_logo -arch=amd64 )
if exist %vs_pro17% ( call %vs_pro17% -no_logo -arch=amd64 )
if exist %vs_com17% ( call %vs_com17% -no_logo -arch=amd64 )

devenv bin\GHOST.exe

endlocal
