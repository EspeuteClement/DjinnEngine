@echo off
setlocal

reg Query "HKLM\Hardware\Description\System\CentralProcessor\0" | find /i "x86" > NUL && (set ARCH=x86) || (set ARCH=x64)

set ROOT=engine
set TCC=%ROOT%\tcc
set CC=%TCC%\tcc.exe
set SDL=%ROOT%\sdl

set SRC=%ROOT%\djn_engine.c

echo %CC%

set PATH=%PATH%;%SDL%\lib\%ARCH%

REM %CC% -E -DSDL_MAIN_HANDLED -I%SDL%\include -L%SDL%\lib\%ARCH% -lSDL2 test.c > debug.c

REM %CC% -DSDL_MAIN_HANDLED -D_STDINT_H_ -I%SDL%\include -L%SDL%\lib\%ARCH% -lSDL2 -run test.c

%CC% -o djn_engine.exe -DSDL_MAIN_HANDLED -D_STDINT_H_ -I%SDL%\include -I%TCC%\libtcc -L%TCC%\libtcc -llibtcc -L%SDL%\lib\%ARCH% -lSDL2 -L%ROOT%\cimgui -lcimgui %ROOT%\cimgui\djn_imgui.c %SRC% 
rem %CC% -E -DSDL_MAIN_HANDLED -D_STDINT_H_ -I%SDL%\include -I%TCC%\libtcc -L%tcc%\libtcc -llibtcc -L%SDL%\lib\%ARCH% -lSDL2 -Lcimgui -lcimgui %SRC% 
