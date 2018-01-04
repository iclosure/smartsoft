@echo off

@echo build.bat:

cd /d %~dp0

:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
:: set parameters
::set ProjectDir=%1
set ProjectDir=%~dp0
set DestDir=%2
set InstallUrl=%3

rem ----------------------------------------
rem make dir
rem ----------------------------------------

if not exist "%DestDir%" (
    md "%DestDir%"
)

rem ----------------------------------------
rem copy and move files
rem ----------------------------------------

copy "%ProjectDir%\qmldir" "%DestDir%"

rem ----------------------------------------
rem delete header files
rem ----------------------------------------

set REMOVE_FILE_SUFFIX=^
    exp, ^
    ilk, ^
    lib

for %%j in ( %REMOVE_FILE_SUFFIX% ) do (
    if exist "%DestDir%\*.%%j" (
        del /a /q /s /f "%DestDir%\*.%%j"
    )
)

:end
exit /b 0
