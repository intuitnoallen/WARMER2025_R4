@echo off

if "%1"=="" goto SHOW_SYNTAX
if "%1"=="latest" goto GET_LATEST

git checkout %1
goto DONE

:GET_LATEST
git checkout develop_tung
goto DONE

:SHOW_SYNTAX
echo.
echo Syntax: switch {version}
echo.
echo version:
echo    x.y.z       Checkout the firmware version x.y.z
echo    latest      Checkout latest firmware
echo.
echo Example: switch 1.7.4

:DONE
echo.
