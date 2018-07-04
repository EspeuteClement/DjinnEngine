# Djinn
Minimalist but powerful game framework in C/C++

## Features
* Live code reload
* Fast build times
* No Classes

## Features

# How to Build
## Windows
0. Install Visual Studio, any edition
1. Open a terminal
2. Find the vcvarall.bat or VsDevCmd.bat file in the Visual Studio install folder and run it with `x86` as argument in your terminal.
3. Run `build.bat`
4. Launch with `lauch.bat`
5. While the game is running, you can run `build.bat` again to reload the code defined in `game.cpp`

## General
* Build `[platform]_platform.cpp`, link against SDL and SDLMain
* Build `game.cpp`, link against SDL
# Libraries
SDL2

Inspired by the amazing work of Casey Muratori @cmuratori