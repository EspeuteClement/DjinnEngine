@echo off

echo %time%

set LIBS= -lglfw
rem set FLAGS= -s USE_WEBGL2=1 -s USE_GLFW=2 -O2 -fno-rtti -fno-exceptions -s ALLOW_MEMORY_GROWTH=1
set FLAGS= -s USE_WEBGL2=1 -s USE_GLFW=3 -s ALLOW_MEMORY_GROWTH=1 -s ASSERTIONS=2 -s TOTAL_MEMORY=134217728 -g4 --source-map-base http://localhost:8000/build/ -std=c++11

IF NOT EXIST %~dp0build mkdir %~dp0build
IF NOT EXIST %~dp0build/data mkdir %~dp0build/data
call build.data.bat

rem pushd %~dp0build
del /Q *.pdb > NUL 2> NUL
call em++ -D__TARGET_WEB__ -D__STANDALONE__ %LIBS%  %FLAGS% src/game.all.cpp src/web_platform.cpp -o build/web.html --preload-file build/data@/data
echo === Finished building game at %time%
echo %time%
rem popd

rem build\win32_platform.exe

