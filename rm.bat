@echo off

SET v=0

:begin

if "%1" == "-f" goto :begin-end
if "%1" == "-v" (
    SET v=1
    goto :begin-end
)

if "%1" == "" goto :end

SET "f=%1"
SET "f=%f:/=\%"
if exist "%f%" (
    del "%f%"
    if "%v%"=="1" echo removed: '%1'
)

:begin-end
shift
goto :begin
:end