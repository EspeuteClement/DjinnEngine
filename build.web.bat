@echo off

clang engine/djn_unity_build.c -Lengine/sdl/lib/x64 -lsdl2 -Iengine/sdl/include -Iengine/ -Wno-pragma-pack -Wno-macro-redefined