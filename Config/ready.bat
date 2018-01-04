
@echo off

title 为工程开发准备库环境

cd /d "%~dp0\.."

rem ****************************************
rem   variables
rem ****************************************

% 项目目录 %
set BASE_PATH=%cd%\\

rem 设置SDK环境变量SMARTKITS_HOME

ver | find "4.0."  > NUL &&  goto set_env_wxp   
ver | find "4.10." > NUL &&  goto set_env_wxp 
ver | find "4.90." > NUL &&  goto set_env_wxp 
ver | find "3.51." > NUL &&  goto set_env_wxp 
ver | find "5.0."  > NUL &&  goto set_env_wxp 
ver | find "5.1."  > NUL &&  goto set_env_wxp 
ver | find "5.2."  > NUL &&  goto set_env_wxp 
ver | find "6.0."  > NUL &&  goto set_env_win7_8
ver | find "6.1."  > NUL &&  goto set_env_win7_8
ver | find "6.2."  > NUL &&  goto set_env_win7_8
ver | find "6.3."  > NUL &&  goto set_env_win7_8

:set_env_wxp

wmic ENVIRONMENT where "name='SMARTKITS_HOME'" delete
wmic ENVIRONMENT create name="SMARTKITS_HOME",username="<system>",VariableValue="%cd%\\"

cd /d "%~sdp0\.."
wmic ENVIRONMENT where "name='SMARTKITS_DIRS'" delete
wmic ENVIRONMENT create name="SMARTKITS_DIRS",username="<system>",VariableValue="%cd%\\"
cd /d "%~dp0\.."

for /f "tokens=3,*" %%i in ('reg query "HKEY_LOCAL_MACHINE\SOFTWARE\Microsoft\WDKDocumentation\7600.091201\Setup" /v "BUILD"') do (
	echo "%%i"
	wmic ENVIRONMENT where "name='WDKPATH'" delete
	wmic ENVIRONMENT create name="WDKPATH",username="<system>",VariableValue="%%i"
)

goto next
 
:set_env_win7_8

@setx SMARTKITS_HOME "%cd%\\"
echo "%BASE_PATH%"

cd /d "%~sdp0\.."
@setx SMARTKITS_DIRS "%cd%\\"
cd /d "%~dp0\.."

for /f "tokens=3,*" %%i in ('reg query "HKEY_LOCAL_MACHINE\SOFTWARE\Microsoft\WDKDocumentation\7600.091201\Setup" /v "BUILD"') do (
	@setx WDKPATH "%%i\"
	echo "%%i\"
)

:next

% 目标文件后缀名 %
set TARGET_FILE_SUFFIX=^
h

% 需要排除的文件名 %
set REMOVE_FILE_NAMES=^
stdafx.h, ^
precomp.h, ^
resource.h, ^
targetver.h, ^
*_p.h

rem ****************************************
rem   copying some files
rem ****************************************

echo.
echo copying some files
echo.

setlocal enabledelayedexpansion

for /f "eol=# tokens=* delims=/n" %%a in (%~sdp0\project.txt) do (
	set line=%%a
	set "line=!line: =!"
	for /f "tokens=1,2 delims=," %%a in ("!line!") do (
		if {%%a} == {} (
			echo error: path = ""
			goto pause
		)
		if {%%b} == {} (
			echo error: path = ""
			goto pause
		)

		if exist "%BASE_PATH%"\src\%%a (
			cd /d "%BASE_PATH%"\src\%%a
			if not exist "%BASE_PATH%"\%%b (
				md "%BASE_PATH%"\%%b
			)
			for %%i in ( %TARGET_FILE_SUFFIX% ) do (
				if exist *.%%i (
					copy /y *.%%i "%BASE_PATH%"\%%b
				)
			)

			cd /d "%BASE_PATH%"\%%b
			for %%j in ( %REMOVE_FILE_NAMES% ) do (
				if exist %%j (
					del /a /q /s /f %%j
				)
			)
		)
	)
)

goto exit

:pause
pause

:exit
exit /b 0