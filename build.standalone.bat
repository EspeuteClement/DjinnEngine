@echo off

echo %time%

set LIBS=Gdi32.lib kernel32.lib user32.lib Shell32.lib Winmm.lib
set FLAGS=/O2 /INCREMENTAL:NO -FC

IF NOT EXIST %~dp0build mkdir %~dp0build
pushd %~dp0build
del /Q *.pdb > NUL 2> NUL
cl /FeStandalone /D_GLFW_WIN32 /D__STANDALONE__ %FLAGS% ../src/game.all.cpp ../src/win32_platform.cpp ../src/external/external.all.c ../src/resources/icon.res %LIBS% /nologo /link /INCREMENTAL:NO
echo === Finished building game at %time%
echo %time%
popd

rem build\win32_platform.exe

