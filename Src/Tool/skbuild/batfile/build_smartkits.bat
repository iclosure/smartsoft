@echo off

echo ------ start build smartkits project ------

rem ----------------------------------------
rem set variables
rem ----------------------------------------
	
set BuildEvent=%1
set SolutionDir=%2
set ProjectDir=%3
set SourceDir=%4
set ProjectName=%5
set TargetName=%6

if "%TargetName%" == "" ( goto exit )

if "%BuildEvent%" == "-sk_pre" (
	
	rem ----------------------------------------
	rem before building: call buildex.bat if exists
	rem ----------------------------------------
	
	if exist %ProjectDir%\buildex.bat (
		call %ProjectDir%\buildex.bat -pre %SolutionDir% %ProjectDir% %SourceDir% %ProjectName% %TargetName%
	)
	
	goto exit
	
) else if "%BuildEvent%" == "-sk_post" (

	rem ----------------------------------------
	rem delete whole folder
	rem ----------------------------------------

	if exist "%SolutionDir%\..\VC\%ProjectName%\include"              ( rd /s /q "%SolutionDir%\..\VC\%ProjectName%\include" )
	if exist "%SolutionDir%\..\VC\%ProjectName%\lib\%TargetName%.lib" ( del /a /q /s /f "%SolutionDir%\..\VC\%ProjectName%\lib\%TargetName%.lib" )
	if exist "%SolutionDir%\..\VC\%ProjectName%\lib\%TargetName%.pdb" ( del /a /q /s /f "%SolutionDir%\..\VC\%ProjectName%\lib\%TargetName%.pdb" )
	if exist "%SolutionDir%\..\VC\%ProjectName%\lib\%TargetName%.dll" ( del /a /q /s /f "%SolutionDir%\..\VC\%ProjectName%\lib\%TargetName%.dll" )

	rem ----------------------------------------
	rem make dir
	rem ----------------------------------------

	if not exist "%SolutionDir%\..\VC\%ProjectName%\include" ( mkdir "%SolutionDir%\..\VC\%ProjectName%\include" )
	if not exist "%SolutionDir%\..\VC\%ProjectName%\lib"     ( mkdir "%SolutionDir%\..\VC\%ProjectName%\lib" )

	rem ----------------------------------------
	rem copy files
	rem ----------------------------------------

	if exist "%SourceDir%\%TargetName%.lib" ( copy "%SourceDir%\%TargetName%.lib" "%SolutionDir%\..\VC\%ProjectName%\lib" )
	if exist "%SourceDir%\%TargetName%.pdb" ( copy "%SourceDir%\%TargetName%.pdb" "%SolutionDir%\..\VC\%ProjectName%\lib" )
	if exist "%SourceDir%\%TargetName%.dll" ( copy "%SourceDir%\%TargetName%.dll" "%SolutionDir%\..\VC\%ProjectName%\lib" )

	rem ----------------------------------------
	rem copy header files
	rem ----------------------------------------
	
	if exist "%ProjectDir%*.h" ( copy "%ProjectDir%*.h" "%SolutionDir%\..\VC\%ProjectName%\include" )

	rem ----------------------------------------
	rem delete some header files
	rem ----------------------------------------
	
	if exist "%SolutionDir%\..\VC\%ProjectName%\include\*_p.h"       (del /a /q /s /f "%SolutionDir%\..\VC\%ProjectName%\include\*_p.h" )
	if exist "%SolutionDir%\..\VC\%ProjectName%\include\resource.h"  (del /a /q /s /f "%SolutionDir%\..\VC\%ProjectName%\include\resource.h" )
	if exist "%SolutionDir%\..\VC\%ProjectName%\include\stdafx.h"    (del /a /q /s /f "%SolutionDir%\..\VC\%ProjectName%\include\stdafx.h" )
	if exist "%SolutionDir%\..\VC\%ProjectName%\include\targetver.h" (del /a /q /s /f "%SolutionDir%\..\VC\%ProjectName%\include\targetver.h" )
	
	rem ----------------------------------------
	rem after building: call buildex.bat if exists
	rem ----------------------------------------

	if exist "%ProjectDir%\buildex.bat" (
		call %ProjectDir%\buildex.bat -post %SolutionDir% %ProjectDir% %SourceDir% %ProjectName% %TargetName%
	)
	
	goto exit
	
) else (

	echo not eupported buildEvent ("%BuildEvent%")
	goto exit
	
)

:exit
exit /b 0
