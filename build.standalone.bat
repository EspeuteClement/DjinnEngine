@echo off

echo %time%

set LIBS=Gdi32.lib kernel32.lib user32.lib Shell32.lib Winmm.lib
set FLAGS=/Zi /INCREMENTAL:NO -FC

IF NOT EXIST %~dp0build mkdir %~dp0build
pushd %~dp0build
del /Q *.pdb > NUL 2> NUL
cl /FeStandalone /D_GLFW_WIN32 /D__STANDALONE__ %FLAGS% ../src/game.cpp ../src/external/ugl.cpp ../src/win32_platform.cpp ../src/external/external.all.c %LIBS% /nologo /link /INCREMENTAL:NO /PDB:game.%random%.pdb
echo === Finished building game at %time%
echo %time%
popd

rem build\win32_platform.exe

