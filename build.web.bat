@echo off

echo %time%

set LIBS=
set FLAGS= -s USE_WEBGL2=1 -s USE_GLFW=3

IF NOT EXIST %~dp0build mkdir %~dp0build
pushd %~dp0build
del /Q *.pdb > NUL 2> NUL
em++ -D__TARGET_WEB__ -D__STANDALONE__ %FLAGS%  ../src/game.cpp ../src/testglfw.cpp %LIBS%
echo === Finished building game at %time%
echo %time%
popd

rem build\win32_platform.exe

