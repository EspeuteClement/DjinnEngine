#ifndef _DJN_IMGUI_H_
#define _DJN_IMGUI_H_

#define WITH_IMGUI

#include <stdlib.h>
#include <stdio.h>
#include <SDL.h>

#define CIMGUI_DEFINE_ENUMS_AND_STRUCTS
#include "cimgui.h"

void 	djn_imgui_init();
void 	djn_imgui_deinit();
void 	djn_imgui_new_frame();
bool 	djn_imgui_process_events(SDL_Event* event);
void    djn_imgui_draw_data(ImDrawData* draw_data);

#endif