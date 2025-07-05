@echo off
setlocal

@if not defined SEVENZIP set SEVENZIP=7z.exe

for /f "delims=" %%a in ('powershell -NoProfile -Command "[System.Runtime.InteropServices.RuntimeInformation]::OSDescription.Trim()"') do set SYSNAME=%%a
for /f "delims=" %%a in ('powershell -NoProfile -Command "[System.Runtime.InteropServices.RuntimeInformation]::OSArchitecture.ToString().Trim()"') do set ARCH=%%a

set SYSNAME=%SYSNAME: =_%
set ARCH=%ARCH: =%

set RELEASE_DIR=release
set ZIP_NAME=%RELEASE_DIR%-%SYSNAME%-%ARCH%.zip

if /I "%1"=="clean" (
    rmdir /s /q "%RELEASE_DIR%"
    if exist "%ZIP_NAME%" del "%ZIP_NAME%"
    goto :EOF
)

if exist "%RELEASE_DIR%" rmdir /s /q "%RELEASE_DIR%"
mkdir "%RELEASE_DIR%"

for %%f in (*.exe *.dll) do (
    copy "%%f" "%RELEASE_DIR%" >nul
)

if exist config.lua copy config.lua "%RELEASE_DIR%" >nul
if exist logic.lua copy logic.lua "%RELEASE_DIR%" >nul

"%SEVENZIP%" a -tzip "%ZIP_NAME%" "%RELEASE_DIR%\*"

endlocal
