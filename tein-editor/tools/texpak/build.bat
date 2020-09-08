@echo off
cl -EHsc -Fetexpak.exe texpak.cpp -link setargv.obj
del texpak.obj