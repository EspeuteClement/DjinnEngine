// Please add to your include path :
// engine/sdl/include 
// engine/

// Link against :
// SDL2
// You can find it here : engine/sdl/lib/x64

#define SDL_MAIN_HANDLED

#define _CRT_SECURE_NO_WARNINGS 1

#include "ugl/ugl.c"
#include "djn_graph.c"

#include "djn_engine.c"
#include "djn_debug.c"
#include "djn_manager.c"
#include "djn_alloc.c"
#include "djn_resources.c"


#ifdef DJN_NO_RELOAD
#include "../game/game_main.c"
#include "../game/game_resources.c"
#endif

#ifndef DJN_NO_IMGUI
#include "cimgui/djn_imgui.c"
#endif