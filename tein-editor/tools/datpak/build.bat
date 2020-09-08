@echo off
cl -EHsc -Fedatpak.exe datpak.cpp -link setargv.obj
del datpak.obj