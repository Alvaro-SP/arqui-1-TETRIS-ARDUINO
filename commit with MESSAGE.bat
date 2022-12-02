
@REM cd "C:\Users\spuser\OneDrive - Facultad de Ingeniería de la Universidad de San Carlos de Guatemala\5.1 VAQUERAS JUNIO 2022\LAB COMPILADORES\PROYECTO-COMPI2"
@echo off
echo %~dp0
SET /P NOMBRE=WRITE COMMIT:
git add .
git commit -m "%NOMBRE%"
git push
pause


