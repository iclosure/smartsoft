
@echo off

title 清除工程垃圾文件

ECHO.
ECHO clean.bat: Deleting output
ECHO.

rem ****************************************
rem   variables
rem ****************************************

% 项目目录 %
SET BASE_PATH=%~sdp0

% 工程文件夹名 %
SET PROJ_NAMES=^
., ^
Kernel\krnlcom, ^
Kernel\krnlprop, ^
Tool\skbuild, ^
Qt, ^
VC\bcgcbpro, ^
VC\common, ^
VC\encrypt, ^
VC\gridctrl, ^
VC\rmtlib, ^
VC\skinctrl, ^
VC\tinyxml, ^
VC\vccore

% 目标文件夹名 %
SET TARGET_FOLDER_NAMES=^
ipch, ^
debug, ^
release, ^
unicode_debug, ^
unicode_release, ^
unicod~1, ^
unicod~2, ^
unicod~3, ^
unicod~4, ^
objchk_wxp_x86, ^
objchk_wlh_x86, ^
objchk_win7_x86, ^
objchk_wxp_x64, ^
objchk_wlh_x64, ^
objchk_win7_x64, ^
objfre_wxp_x86, ^
objfre_wlh_x86, ^
objfre_win7_x86, ^
objfre_wxp_x64, ^
objfre_wlh_x64, ^
objfre_win7_x64

% 目标文件名 %
SET TARGET_FILE_NAMES=^
aps, ^
ncb, ^
suo,^
sdf, ^
opensdf, ^
user, ^
idb, ^
pdb, ^
map, ^
log, ^
wrn, ^
err

rem ****************************************
rem   Delete working folders
rem ****************************************

ECHO.
ECHO clean.bat: Deleting working folders
ECHO.

FOR %%I IN ( %PROJ_NAMES% ) DO (
	CD "%BASE_PATH%\%%I"
	FOR %%J IN ( %TARGET_FOLDER_NAMES% ) DO (
		IF EXIST "%%J" (
			IF {"%%I"} == {"."} (
				ECHO Delete folder: "%BASE_PATH%\%%J"
			) ELSE (
				ECHO Delete folder: "%BASE_PATH%\%%I\%%J"
			)
			RD /Q /S "%%J"
		)
	)
	CD "%BASE_PATH%"
)

rem ****************************************
rem   Delete files
rem ****************************************

ECHO.
ECHO clean.bat: Deleting files
ECHO.

FOR %%I IN ( %PROJ_NAMES% ) DO (
	CD %BASE_PATH%\%%I
	FOR %%J IN ( %TARGET_FILE_NAMES% ) DO ( 
		FOR %%K IN ( *.%%J ) DO (
			IF EXIST %%K (
				IF {%%I} == {.} ( 
					ECHO Delete file: %BASE_PATH%\%%K
				) ELSE (
					ECHO Delete file: %BASE_PATH%\%%I\%%K
				)
				DEL /A /Q /S /F %%K
			)
		)
	)
	CD %BASE_PATH%
)

::pause
