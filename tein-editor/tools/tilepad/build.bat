@echo off
cl -EHsc -Fetilepad.exe tilepad.c -link setargv.obj
del tilepad.obj