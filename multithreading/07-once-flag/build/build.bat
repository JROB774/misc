@echo off
setlocal

pushd ..

call build\utility\findvsdev.bat
call build\utility\buildvars.bat

call %VSDevPath% -no_logo -arch=%Architecture%

if not exist binary mkdir binary
pushd binary

call ..\build\utility\buildtime.bat "cl %IncludeDirs% %Defines% %CompilerFlags% %CompilerWarnings% -Fe%OutputExecutable% %InputSource% -link %LinkerFlags% %LinkerWarnings% %LibraryDirs% %Libraries%"

del *.ilk *.res *.obj *.exp *.lib

popd
popd

endlocal
