@echo off

set InputSource=main
set OutputROM=test

..\.gbdk\bin\lcc -Wa-l -Wl-m -Wl-j -DUSE_SFR_FOR_REG -c -o %InputSource%.o %InputSource%.c
..\.gbdk\bin\lcc -Wa-l -Wl-m -Wl-j -DUSE_SFR_FOR_REG -o %OutputROM%.gb %InputSource%.o

del *.lst *.map *.o *.sym
