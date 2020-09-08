@echo off

pushd bin
del "*.log"
for %%i in (*.exe) do start "" /b "%%i" break
popd bin
