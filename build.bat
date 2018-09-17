cls
@echo off
setlocal

reg Query "HKLM\Hardware\Description\System\CentralProcessor\0" | find /i "x86" > NUL && (set ARCH=x86) || (set ARCH=x64)

set ROOT=engine
set TCC=%ROOT%\tcc
set CC=%TCC%\tcc.exe
set SDL=%ROOT%\sdl

set MAIN=%ROOT%\djn_engine.c
set SRC= %ROOT%\cimgui\djn_imgui.c %ROOT%\djn_resources.c %ROOT%\djn_alloc.c
echo %CC%

set PATH=%PATH%;%SDL%\lib\%ARCH%

%CC% -o djn_engine.exe -DSDL_MAIN_HANDLED -D_STDINT_H_ -I%SDL%\include -I%TCC%\libtcc -L%TCC%\libtcc -llibtcc -L%SDL%\lib\%ARCH% -lSDL2 -L%ROOT%\cimgui -lcimgui %SRC% %MAIN% 
