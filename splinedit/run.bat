@echo off

pushd bin
for %%i in (*.exe) do start "" /b "%%i" break
popd bin