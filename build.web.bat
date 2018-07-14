@echo off

echo %time%

set LIBS= -lglfw
set FLAGS= -s USE_WEBGL2=1 -s USE_GLFW=2 -s ASSERTIONS=2

IF NOT EXIST %~dp0build mkdir %~dp0build
pushd %~dp0build
del /Q *.pdb > NUL 2> NUL
call em++ -D__TARGET_WEB__ -D__STANDALONE__ %LIBS%  ../src/game.cpp ../src/web_platform.cpp ../src/external/ugl.cpp %FLAGS% -o web.html --preload-file data
echo === Finished building game at %time%
echo %time%
popd

rem build\win32_platform.exe

