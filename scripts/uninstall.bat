@echo off

set location=C:\bin
@set /p location=Location of install (%location%): 

mkdir "%location%" 2> NUL

choice /N /M "Remove everything (additional data included)? (y/n) "

if %errorlevel% equ 1 echo Deleting data...
echo Uninstalling libraries...
del "%location%\libppc-*.dll" 2> NUL
echo Uninstalling ++C...
del "%location%\++c.exe" 2> NUL
@REM else set everything=no


@REM choice /N /M "Add location to PATH? (y/n) "

@REM if %errorlevel% equ 1 setx PATH "%PATH%;%location%"