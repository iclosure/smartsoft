@echo off

echo ------ start build kernel project ------

setlocal enabledelayedexpansion

rem ----------------------------------------
rem set variables
rem ----------------------------------------
	
set BuildEvent=%1
set SolutionDir=%2
set ProjectDir=%3

if "%ProjectDir%" == "" ( goto exit )

if "%BuildEvent%" == "-krnl_post" (

	rem ----------------------------------------
	rem set variables
	rem ----------------------------------------
	
	set SourceDir=%4
	set ProjectName=%5
	set TargetName=%6

	if "!TargetName!" == "" ( goto exit )
	
	rem ----------------------------------------
	rem delete whole folder
	rem ----------------------------------------

	if exist "%SolutionDir%\..\Kernel\!ProjectName!\include"              ( rd /s /q "%SolutionDir%\..\Kernel\!ProjectName!\include" )
	if exist "%SolutionDir%\..\Kernel\!ProjectName!\lib\!TargetName!.lib" ( del /a /q /s /f "%SolutionDir%\..\Kernel\!ProjectName!\lib\!TargetName!.lib" )
	if exist "%SolutionDir%\..\Kernel\!ProjectName!\lib\!TargetName!.pdb" ( del /a /q /s /f "%SolutionDir%\..\Kernel\!ProjectName!\lib\!TargetName!.pdb" )
	if exist "%SolutionDir%\..\Kernel\!ProjectName!\lib\!TargetName!.dll" ( del /a /q /s /f "%SolutionDir%\..\Kernel\!ProjectName!\lib\!TargetName!.dll" )

	rem ----------------------------------------
	rem make dir
	rem ----------------------------------------

	if not exist "%SolutionDir%\..\Kernel\!ProjectName!"         ( mkdir "%SolutionDir%\..\Kernel\!ProjectName!" )
	if not exist "%SolutionDir%\..\Kernel\!ProjectName!\include" ( mkdir "%SolutionDir%\..\Kernel\!ProjectName!\include" )
	if not exist "%SolutionDir%\..\Kernel\!ProjectName!\lib"     ( mkdir "%SolutionDir%\..\Kernel\!ProjectName!\lib" )

	rem ----------------------------------------
	rem copy files
	rem ----------------------------------------

	if exist "!SourceDir!\!TargetName!.lib" ( copy "!SourceDir!\!TargetName!.lib" "%SolutionDir%\..\Kernel\!ProjectName!\lib" )
	if exist "!SourceDir!\!TargetName!.pdb" ( copy "!SourceDir!\!TargetName!.pdb" "%SolutionDir%\..\Kernel\!ProjectName!\lib" )
	if exist "!SourceDir!\!TargetName!.dll" ( copy "!SourceDir!\!TargetName!.dll" "%SolutionDir%\..\Kernel\!ProjectName!\lib" )

	rem ----------------------------------------
	rem copy header files
	rem ----------------------------------------
	
	if exist "%ProjectDir%*.h" ( copy "%ProjectDir%*.h" "%SolutionDir%\..\Kernel\!ProjectName!\include" )

	rem ----------------------------------------
	rem delete some header files
	rem ----------------------------------------

	if exist "%SolutionDir%\..\Kernel\!ProjectName!\include\resource.h"  (del /a /q /s /f "%SolutionDir%\..\Kernel\!ProjectName!\include\resource.h" )
	if exist "%SolutionDir%\..\Kernel\!ProjectName!\include\stdafx.h"    (del /a /q /s /f "%SolutionDir%\..\Kernel\!ProjectName!\include\stdafx.h" )
	if exist "%SolutionDir%\..\Kernel\!ProjectName!\include\targetver.h" (del /a /q /s /f "%SolutionDir%\..\Kernel\!ProjectName!\include\targetver.h" )
	
	goto exit
	
) else if "%BuildEvent%" == "-krnl_lib" (

	rem ----------------------------------------
	rem set variables
	rem ----------------------------------------
	
	set ProjectName=%4
	set TargetName=%5
	set ConfigType=%6
	set SystemName=%7
	set PlatformName=%8
	set BuildType=%9
	
	if "!PlatformName!" == "" (
		echo the count of parameters are not enough, and must be not less than 8 for '%BuildEvent%'
		goto exit
	) else if "!PlatformName!" == "x86" (
		set PlatformFolder=i386
		goto krnl_lib_config_type
	) else (
		echo not supported platform ( "!PlatformName!" )
		goto exit
	)

	:krnl_lib_config_type
	
	if "!ConfigType!" == "chk" (
		set ConfigTypeFolder=Debug
		goto krnl_lib_start
	) else if "!ConfigType!" == "fre" (
		set ConfigTypeFolder=Release
		goto krnl_lib_start
	) else (
		echo ConfigType - !ConfigType! must be either chk and fre
		goto exit
	)
	
	:krnl_lib_start
	
	set TargetFolder=obj!ConfigType!_!SystemName!_!PlatformName!\!PlatformFolder!
	set SourceDir=%ProjectDir%\!TargetFolder!
	set TargetLibDir=%SolutionDir%\..\Kernel\!ProjectName!\lib\!TargetFolder!
	
	rem ----------------------------------------
	rem check parameters
	rem ----------------------------------------
	
	if "!BuildType!" == "-clear" (
		goto DriverClean
	)
	
	rem ----------------------------------------
	rem delete some folders
	rem ----------------------------------------
	
	for %%I in ( obj, !TargetFolder! ) do (
		if exist "%ProjectDir%\%%I" (
			rd /q /s "%ProjectDir%\%%I"
		)
	)
	
	rem ----------------------------------------
	rem before building: call buildex.bat if exists
	rem ----------------------------------------
	
	if exist %ProjectDir%\buildex.bat (
		call %ProjectDir%\buildex.bat -pre %ProjectDir% !SourceDir! !TargetName! !ConfigType! !SystemName! !PlatformName! !ConfigTypeFolder! !TargetFolder!
	)
	
	rem ----------------------------------------
	rem make driver
	rem ----------------------------------------
	
	call %WDKPATH%\bin\setenv.bat %WDKPATH% !ConfigType! !PatformName! !SystemName!
	echo %ProjectDir%
	cd /d %ProjectDir%

	echo project path: %ProjectDir%
	echo target: !TargetName!

	if "!BuildType!" == "" (
		build
	) else if "!BuildType!" == "-rebuild" (
		build -cfeg
	)
	
	rem ----------------------------------------
	rem delete whole folder
	rem ----------------------------------------
	
	if exist "%SolutionDir%\..\Kernel\!ProjectName!\include" ( rd /s /q "%SolutionDir%\..\Kernel\!ProjectName!\include" )
	if exist "!TargetLibDir!\!TargetName!.lib" ( del /a /q /s /f "!TargetLibDir!\!TargetName!.lib" )
	if exist "!TargetLibDir!\!TargetName!.pdb" ( del /a /q /s /f "!TargetLibDir!\!TargetName!.pdb" )
	if exist "!TargetLibDir!\!TargetName!.dll" ( del /a /q /s /f "!TargetLibDir!\!TargetName!.dll" )
	
	rem ----------------------------------------
	rem make dir
	rem ----------------------------------------
	
	if not exist "%SolutionDir%\..\Kernel\!ProjectName!\include" ( @mkdir "%SolutionDir%\..\Kernel\!ProjectName!\include" )
	if not exist "!TargetLibDir!" ( mkdir "!TargetLibDir!" )
	
	rem ----------------------------------------
	rem copy files
	rem ----------------------------------------
	
	if exist "!SourceDir!\!TargetName!.lib" ( copy "!SourceDir!\!TargetName!.lib" "!TargetLibDir!" )
	if exist "!SourceDir!\!TargetName!.pdb" ( copy "!SourceDir!\!TargetName!.pdb" "!TargetLibDir!" )
	if exist "!SourceDir!\!TargetName!.dll" ( copy "!SourceDir!\!TargetName!.dll" "!TargetLibDir!" )
	
	rem ----------------------------------------
	rem copy header files
	rem ----------------------------------------
	
	if exist "%ProjectDir%*.h" ( copy "%ProjectDir%*.h" "%SolutionDir%\..\Kernel\!ProjectName!\include" )
	
	rem ----------------------------------------
	rem after building: call buildex.bat if exists
	rem ----------------------------------------
	
	if exist "%ProjectDir%\buildex.bat" (
		call %ProjectDir%\buildex.bat -post %ProjectDir% !SourceDir! !TargetName! !ConfigType! !SystemName! !PlatformName! !ConfigTypeFolder! !TargetFolder!
	)
	
	goto exit
	
	rem ----------------------------------------
	rem clean some folders and files
	rem ----------------------------------------
	
	:DriverClean
	for %%I in ( obj, obj!ConfigType!_!SystemName!_!PlatformName! ) do (
		if exist "!ProjectDir!\%%I" (
			echo delete "!ProjectDir!\%%I"
			rd /q /s "!ProjectDir!\%%I"
		)
	)
	
	for %%I in ( log, wrn, err ) do ( 
		for %%J in ( *.%%I ) do (
			if exist "!ProjectDir!\%%J" (
				del /a /q /s /f "!ProjectDir!\%%J"
			)
		)
	)
	
	goto exit
	
) else (

	echo not eupported buildEvent ("%BuildEvent%")
	goto exit
	
)

:exit
exit /b 0
