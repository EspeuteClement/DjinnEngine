@echo off
IF NOT EXIST %~dp0build mkdir %~dp0build
IF NOT EXIST %~dp0build\tools mkdir %~dp0build\tools

set FLAGS=/Zi /INCREMENTAL:NO -FC
set LIBS=Gdi32.lib kernel32.lib user32.lib Shell32.lib Winmm.lib

pushd %~dp0build\tools
cl %FLAGS% %LIBS% ../../src/tools/djinn_pack.cpp  /nologo /link /INCREMENTAL:NO
popd