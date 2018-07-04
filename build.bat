@echo off

rem call "D:\Program Files (x86)\Microsoft Visual Studio\2017\Community\VC\Auxiliary\Build\vcvarsall.bat" x86
rem set PATH="D:\Program Files (x86)\Microsoft Visual Studio\2017\Community\VC\Tools\MSVC\14.12.25827\bin\Hostx86\x86\";%PATH%
rem set INCLUDE=INCLUDE="D:\Program Files (x86)\Microsoft Visual Studio\2017\Community\VC\Tools\MSVC\14.12.25827\ATLMFC\include";"D:\Program Files (x86)\Microsoft Visual Studio\2017\Community\VC\Tools\MSVC\14.12.25827\include";"C:\Program Files (x86)\Windows Kits\NETFXSDK\4.6.1\include\um";"C:\Program Files (x86)\Windows Kits\10\include\10.0.16299.0\ucrt";"C:\Program Files (x86)\Windows Kits\10\include\10.0.16299.0\shared";"C:\Program Files (x86)\Windows Kits\10\include\10.0.16299.0\um";"C:\Program Files (x86)\Windows Kits\10\include\10.0.16299.0\winrt";
echo %time%
set LINKDIR=../lib/
set LIBS=%LINKDIR%x86/SDL2.lib %LINKDIR%x86/SDL2Main.lib
set FLAGS=/INCREMENTAL:NO -FC

IF NOT EXIST %~dp0build mkdir %~dp0build
pushd %~dp0build
del /Q *.pdb 2> NUL
cl /Fdgame.%RANDOM%.pdb %FLAGS% /Zi ../src/game.cpp /DLL /LD /nologo /link /INCREMENTAL:NO
cl  %FLAGS% /Zi ../src/win32_platform.cpp %LIBS% /nologo /link /INCREMENTAL:NO
echo %time%
popd

rem build\win32_platform.exe

