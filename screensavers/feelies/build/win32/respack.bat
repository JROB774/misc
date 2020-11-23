:: Pack Binary Resources Script

@echo off
setlocal

pushd ..\..\tools
python binarytocpp.py -o ../source/resources/ ../binary/resources/
popd

endlocal
