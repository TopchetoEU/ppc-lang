@echo off

set location=C:\bin
@set /p location=Location of install (%location%): 

mkdir "%location%" 2> NUL

echo Installing libraries...
xcopy /Q /Y "bin\*.dll" "%location%" > NUL

echo Installing ++C...
echo F | xcopy /Q /Y "%bin%\%output%-windows.exe" "%location%\++c.exe" > NUL

choice /N /M "Add location to PATH? (y/n) "

if %errorlevel% equ 1 powershell -Command "[Environment]::SetEnvironmentVariable('path',\"%location%;$([Environment]::GetEnvironmentVariable('path','Machine'))\",'Machine');"