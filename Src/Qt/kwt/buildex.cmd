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
if not exist "%DestDir%\..\include\serialport" (
    md "%DestDir%\..\include\serialport"
)

rem ----------------------------------------
rem copy header files
rem ----------------------------------------

copy "%ProjectDir%\src\*.h" "%DestDir%\..\include"
copy "%ProjectDir%\src\serialport\*.h" "%DestDir%\..\include\serialport"

rem ----------------------------------------
rem delete header files
rem ----------------------------------------

del /a /q /s /f "%DestDir%\..\include\*_p.h"
