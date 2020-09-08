@echo off
cl -EHsc -Fetilegen.exe tilegen.cpp -link setargv.obj
del tilegen.obj