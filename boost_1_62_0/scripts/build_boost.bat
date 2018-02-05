@ECHO OFF
SETLOCAL

set WORKINGDIR=%1

call "C:\Program Files (x86)\Microsoft Visual Studio 14.0\VC\vcvarsall.bat" amd64

ECHO ###############################################################################
ECHO ########################## -BUILDING BJAM BINARIES- ###########################
ECHO ###############################################################################
SET MYPATH="%CD%"
cd "%~dp0"
if exist .\b2.exe del .\b2.exe
if exist .\bjam.exe del .\bjam.exe
call .\bootstrap.bat
if exist .\bin.v2 rmdir .\bin.v2 /s/q
if exist ..\bin rmdir ..\bin /s/q

rem ECHO ###############################################################################
rem ECHO ########## -BUILDING BOOST LIBRARIES 32bit **RUNTIME-LINK = ALL**- ############
rem ECHO ###############################################################################
rem if exist .\stage_x86 rmdir .\stage_x86 /s/q
rem b2.exe --toolset=msvc-14.0 --clean-all
rem b2.exe --toolset=msvc-14.0 architecture=x86 address-model=32 --stagedir=".\stage_x86" threading=multi --build-type=complete stage
rem if not exist ..\bin\x86 md ..\bin\x86
rem move /y .\stage_x86\lib\*.* ..\bin\x86

rem if exist .\bin.v2 rmdir .\bin.v2 /s/q
ECHO ###############################################################################
ECHO ########## -BUILDING BOOST LIBRARIES 64bit **RUNTIME-LINK = ALL**- ############
ECHO ###############################################################################
if exist .\stage_x64 rmdir \stage_x64 /s/q
ECHO "Cleaning..."
b2.exe --toolset=msvc-14.0 --clean-all
ECHO "Starting to build..."
b2.exe -j6 --toolset=msvc-14.0 architecture=x86 address-model=64 -sBZIP2_SOURCE=%WORKINGDIR%\bzip2 -sZLIB_SOURCE=%WORKINGDIR%\zlib --stagedir=".\stage_x64" --build-type=minimal stage
if not exist .\stage md .\stage
if not exist ..\bin\x64 md ..\bin\x64
move /y .\stage_x64\lib\*.* ..\bin\x64
if exist .\stage_x86 rmdir .\stage_x86 /s/q
if exist .\stage_x64 rmdir .\stage_x64 /s/q

CD %MYPATH%

ECHO "Checking architecture of compiled lib"
"C:\Program Files (x86)\Microsoft Visual Studio 14.0\VC\bin\amd64\dumpbin.exe" /HEADERS "%WORKINGDIR%\bin\x64\libboost_atomic-vc140-mt-1_62.lib" | findstr machine

ENDLOCAL
@ECHO ON
