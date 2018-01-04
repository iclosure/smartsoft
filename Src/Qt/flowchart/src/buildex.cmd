%echo off

cd /d %~dp0

rem ----------------------------------------
rem set parameters
rem ----------------------------------------

set ProjectDir=%~dp0
set DestDir=%1

rem ----------------------------------------
rem make dir
rem ----------------------------------------

if not exist "%DestDir%" (
    md "%DestDir%"
)
if not exist "%DestDir%\..\include" (
    md "%DestDir%\..\include"
)
rem ----------------------------------------
rem copy header files
rem ----------------------------------------

copy "%ProjectDir%\jflowchart.h" "%DestDir%\..\include"

rem ----------------------------------------
rem delete header files
rem ----------------------------------------

del /a /q /s /f "%DestDir%\..\include\*_p.h"
