@echo off

rem ----------------------------------------
rem set variables
rem ----------------------------------------
	
set BuildEvent=%1
set SolutionDir=%2
set ProjectDir=%3
set SourceDir=%4
set ProjectName=%5
set TargetName=%6

if "%BuildEvent%" == "-pre" (

	echo %~nx0: build event bat file

	echo build event: %BuildEvent%
	
) else if "%BuildEvent%" == "-post" (

	rem ----------------------------------------
	rem copy files
	rem ----------------------------------------

	rem -- copy files of crc module --
	if not exist "%SolutionDir%\..\VC\%ProjectName%\include\crc" ( mkdir "%SolutionDir%\..\VC\%ProjectName%\include\crc" )
	copy "%ProjectDir%crc\*.h" "%SolutionDir%\..\VC\%ProjectName%\include\crc"
	
	rem -- copy files of moxa module --
	if not exist "%SolutionDir%\..\VC\%ProjectName%\include\moxa" ( mkdir "%SolutionDir%\..\VC\%ProjectName%\include\moxa" )
	if not exist "%SolutionDir%\..\VC\%ProjectName%\lib\moxa" ( mkdir "%SolutionDir%\..\VC\%ProjectName%\lib\moxa" )
	copy "%ProjectDir%moxa\*.h" "%SolutionDir%\..\VC\%ProjectName%\include\moxa"
	copy "%ProjectDir%moxa\*.lib" "%SolutionDir%\..\VC\%ProjectName%\lib\moxa"
	copy "%ProjectDir%moxa\*.dll" "%SolutionDir%\..\VC\%ProjectName%\lib\moxa"
	
	rem -- copy files of network module --
	if not exist "%SolutionDir%\..\VC\%ProjectName%\include\network" ( mkdir "%SolutionDir%\..\VC\%ProjectName%\include\network" )
	copy "%ProjectDir%network\*.h" "%SolutionDir%\..\VC\%ProjectName%\include\network"
	
	rem -- copy files of serialport module --
	if not exist "%SolutionDir%\..\VC\%ProjectName%\include\serialport" ( mkdir "%SolutionDir%\..\VC\%ProjectName%\include\serialport" )
	copy "%ProjectDir%serialport\*.h" "%SolutionDir%\..\VC\%ProjectName%\include\serialport"
	
	goto exit
	
) else (

	echo not supported build event: ( '%BuildEvent%' )
	goto exit
	
)

goto exit

:exit
exit /b 0
