@echo off

echo %time%

set GAME_NAME=win32_platform

rem === Common Stuff ===
set COMMON_FLAGS=/Zi /INCREMENTAL:NO -FC /nologo
    rem force single non virtual inheritance
    set COMMON_FLAGS=%COMMON_FLAGS% /vms
set COMMON_LINK=/INCREMENTAL:NO

rem == DLL Config ===
set DLL_SOURCES=../src/game.all.cpp
set DLL_FLAGS=%COMMON_FLAGS% /Fe:game.dll /DLL /LDd

rem === Host Config ===
set LIBS=Gdi32.lib kernel32.lib user32.lib Shell32.lib Winmm.lib
set EXE_NAME=%GAME_NAME%.exe

IF NOT EXIST %~dp0build mkdir %~dp0build
IF NOT EXIST %~dp0build/data mkdir %~dp0build/data
call build.data.bat

pushd %~dp0build
IF NOT EXIST tmp mkdir tmp

del /Q *.pdb > NUL 2> NUL
@echo on
cl /D_GLFW_WIN32 %DLL_FLAGS% %DLL_SOURCES% /link %COMMON_LINK% /PDB:game.%random%.pdb
@echo === Finished building game.dll at %time%
cl /Fe%EXE_NAME% %COMMON_FLAGS%  /D_GLFW_WIN32 ../src/win32_platform.cpp ../src/external/external.all.c %LIBS% /link %COMMON_LINK% /PDB:%GAME_NAME%.%random%.pdb
@echo off
echo %time%
popd

rem build\win32_platform.exe

