@echo off

rem call "D:\Program Files (x86)\Microsoft Visual Studio\2017\Community\VC\Auxiliary\Build\vcvarsall.bat" x86
rem set PATH="D:\Program Files (x86)\Microsoft Visual Studio\2017\Community\VC\Tools\MSVC\14.12.25827\bin\Hostx86\x86\";%PATH%
rem set INCLUDE=INCLUDE="D:\Program Files (x86)\Microsoft Visual Studio\2017\Community\VC\Tools\MSVC\14.12.25827\ATLMFC\include";"D:\Program Files (x86)\Microsoft Visual Studio\2017\Community\VC\Tools\MSVC\14.12.25827\include";"C:\Program Files (x86)\Windows Kits\NETFXSDK\4.6.1\include\um";"C:\Program Files (x86)\Windows Kits\10\include\10.0.16299.0\ucrt";"C:\Program Files (x86)\Windows Kits\10\include\10.0.16299.0\shared";"C:\Program Files (x86)\Windows Kits\10\include\10.0.16299.0\um";"C:\Program Files (x86)\Windows Kits\10\include\10.0.16299.0\winrt";
echo %time%
set LINKDIR=../lib/
set LIBS=Gdi32.lib kernel32.lib user32.lib Shell32.lib opengl32.lib Winmm.lib
set FLAGS=/Zi /INCREMENTAL:NO -FC

IF NOT EXIST %~dp0build mkdir %~dp0build
pushd %~dp0build
del /Q *.pdb > NUL 2> NUL
cl  %FLAGS% ../src/game.cpp -I../src/external/include ../src/external/ugl.c /DLL /LD /nologo /link /INCREMENTAL:NO /PDB:game.%random%.pdb
cl  %FLAGS%  /D_GLFW_WIN32 ../src/testglfw.cpp ../src/external/ugl.c ../src/external/external.all.c %LIBS% -I../src/external/include  /nologo /link /INCREMENTAL:NO /PDB:testglfw.%random%.pdb
echo %time%
popd

rem build\win32_platform.exe

