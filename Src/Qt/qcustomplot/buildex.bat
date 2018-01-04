%echo off

echo build.bat: 

cd /d %~dp0

:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
:: set parameters
set ProjectDir=%~dp0
set ProjectName=%1
set SolutionDir=%2
rem ----------------------------------------
rem make dir
rem ----------------------------------------

if not exist "%SolutionDir%\..\bin\%ProjectName%" (
	md "%SolutionDir%\..\bin\%ProjectName%"
)

if not exist "%SolutionDir%\..\inc\%ProjectName%" (
md "%SolutionDir%\..\inc\%ProjectName%"
)

if not exist "%SolutionDir%\..\lib\%ProjectName%" (
md "%SolutionDir%\..\lib\%ProjectName%"
)

rem ----------------------------------------
rem copy and move files
rem ----------------------------------------
copy "lib\*.lib" "%SolutionDir%\..\lib\%ProjectName%"
copy "lib\*.dll" "%SolutionDir%\..\bin\%ProjectName%"
rem ----------------------------------------
rem delete header files
rem ----------------------------------------
del /a /q /s /f "%SolutionDir%\..\inc\%ProjectName%\*.h"
rem ----------------------------------------
rem copy header files
rem ----------------------------------------
copy "%ProjectDir%\src\*.h" "%SolutionDir%\..\inc\%ProjectName%"