@echo off
cl -EHsc -Feautocrop.exe autocrop.cpp -link setargv.obj
del autocrop.obj