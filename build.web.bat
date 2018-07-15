@echo off

echo %time%

set LIBS= -lglfw
set FLAGS= -s USE_WEBGL2=1 -s USE_GLFW=2 -O2 -fno-rtti -fno-exceptions -s ALLOW_MEMORY_GROWTH=1

IF NOT EXIST %~dp0build mkdir %~dp0build
pushd %~dp0build
del /Q *.pdb > NUL 2> NUL
call em++ -D__TARGET_WEB__ -D__STANDALONE__ %LIBS%  %FLAGS% ../src/game.cpp ../src/web_platform.cpp ../src/external/ugl.cpp -o web.html --preload-file data
echo === Finished building game at %time%
echo %time%
popd

rem build\win32_platform.exe

