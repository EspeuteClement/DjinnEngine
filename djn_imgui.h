#ifndef _DJN_IMGUI_H_
#define _DJN_IMGUI_H_
#define CIMGUI_DEFINE_ENUMS_AND_STRUCTS
#include "cimgui/cimgui.h"

void djn_imgui_init();
void djn_imgui_deinit();

void 	ImGui_ImplOpenGL3_NewFrame();
int    ImGui_ImplOpenGL3_Init(const char* glsl_version);
void    ImGui_ImplOpenGL3_Shutdown();
void    ImGui_ImplOpenGL3_RenderDrawData(ImDrawData* draw_data);
void djn_imgui_new_frame();
bool ImGui_ImplSDL2_ProcessEvent(SDL_Event* event);


#endif