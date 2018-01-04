@echo off

set BuildEvent=%1
set ProjectDir=%2
set SourceDir=%3
set TargetName=%4
set ConfigType=%5
set SystemName=%6
set PlatformName=%7
set ConfigTypeFolder=%8
set TargetFolder=%9

if "%BuildEvent%" == "-pre" (

	echo %~nx0: build event bat file

	echo build event: %BuildEvent%
	
) else if "%BuildEvent%" == "-post" (

	del /a /q /s /f "%SMARTKITS_HOME%"\Kernel\krnlcom\include\precomp.h
	
) else (

	echo not supported build event: ( '%BuildEvent%' )
	goto exit
	
)

goto exit

:exit
exit /b 0
