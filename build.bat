cls
@echo off
setlocal

reg Query "HKLM\Hardware\Description\System\CentralProcessor\0" | find /i "x86" > NUL && (set ARCH=x86) || (set ARCH=x64)

set ROOT=engine
set TCC=%ROOT%\tcc
set CC=%TCC%\tcc.exe
set SDL=%ROOT%\sdl

set MAIN=%ROOT%\djn_unity_build.c
echo %CC%

set PATH=%PATH%;%SDL%\lib\%ARCH%

%CC% -o djn_engine.exe -D_STDINT_H_ -I%ROOT% -I%SDL%\include -I%TCC%\libtcc -L%TCC%\libtcc -llibtcc -L%SDL%\lib\%ARCH% -lSDL2 -L%ROOT%\cimgui -lcimgui %MAIN% 
