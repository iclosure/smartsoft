@echo off

echo ------ start build driver project ------

setlocal enabledelayedexpansion

rem ----------------------------------------
rem set variables
rem ----------------------------------------
	
set BuildEvent=%1
set SolutionDir=%2
set ProjectDir=%3

if "%ProjectDir%" == "" ( goto exit )

if "%BuildEvent%" == "-post" (

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
	
) else if "%BuildEvent%" == "-drv" (

	rem ----------------------------------------
	rem set variables
	rem ----------------------------------------
	
	set TargetName=%4
	set ConfigType=%5
	set SystemName=%6
	set PlatformName=%7
	set BuildType=%8
	set WdfConinstallerName=WdfCoInstaller01009
	
	if "!PlatformName!" == "" (
		echo the count of parameters are not enough, and must be not less than 7 for '%BuildEvent%'
		goto exit
	) else if "!PlatformName!" == "x86" (
		set PlatformFolder=i386
		goto krnl_drv_config_type
	) else (
		echo not supported platform ( "!PlatformName!" )
		goto exit
	)

	:krnl_drv_config_type
	
	if "!ConfigType!" == "chk" (
		set ConfigTypeFolder=Debug
		goto drv_start
	) else if "!ConfigType!" == "fre" (
		set ConfigTypeFolder=Release
		goto drv_start
	) else (
		echo ConfigType - !ConfigType! must be either chk and fre
		goto exit
	)

	:drv_start
	
	set TargetFolder=obj!ConfigType!_!SystemName!_!PlatformName!\!PlatformFolder!
	set SourceDir=%ProjectDir%\!TargetFolder!
	
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
	rem refresh version sectin in !TargetName!.inx file
	rem ----------------------------------------
	
	call "%SMARTKITS_HOME%"\Bin\skbuild.exe -drv_inf -ver !ProjectDir! !SourceDir! !TargetName! !ConfigType!

	rem ----------------------------------------
	rem copy files
	rem ----------------------------------------
	
	copy "%WDKPATH%\redist\wdf\!PlatformName!\%WdfConinstallerName%.dll" "!SourceDir!"
	
	rem ----------------------------------------
	rem after building: call buildex.bat if exists
	rem ----------------------------------------
	
	if exist "%ProjectDir%\buildex.bat" (
		call %ProjectDir%\buildex.bat -post %ProjectDir% !SourceDir! !TargetName! !ConfigType! !SystemName! !PlatformName! !ConfigTypeFolder! !TargetFolder!
	)
	
	rem ----------------------------------------
	rem after building: call buildex.bat if exists
	rem ----------------------------------------
	
	"%WDKPATH%\bin\selfsign\Inf2Cat.exe" /driver:"%TargetFolder%" /os:2000,XP_X86,XP_X64,Server2003_X86,Server2003_X64,Vista_X86,Vista_X64,7_X86,7_X64
	
	goto exit
	
	rem ----------------------------------------
	rem clean some folders and files
	rem ----------------------------------------
	
	:DriverClean
	for %%I in ( obj, !TargetFolder! ) do (
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
