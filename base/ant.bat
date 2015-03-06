@echo off
set CYGWIN=nodosfilewarning
set PATH=%PATH%;D:\usr\viseml\git\microcontroller\build\win

PowerShell -file ant.ps1 %1 %2

