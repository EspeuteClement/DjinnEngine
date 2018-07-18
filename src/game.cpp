#include "game.h"
#include <cstdio>

#include "graph.cpp"
#include "external/imgui/imgui.h"
#include "external/imgui/imgui_impl_djinn.h"

static const struct
{
    float x, y;
    float r, g, b;
} vertices[3] =
{
    { -0.6f, -0.4f, 1.f, 0.f, 1.f },
    {  0.6f, -0.4f, 0.f, 1.f, 1.f },
    {   0.f,  0.6f, 1.f, 1.f, 0.f }
};

static const struct
{
    float x, y;
    float r, g, b;
    float u, v;
} new_vertices[6] =
{
    {   0.0f, 0.0f, 1.f, 1.f, 1.f, 0.0f, 0.0f },
    {   0.0f, 240.0f, 1.f, 1.f, 1.f, 0.0f, 1.0f },
    {   512.0f, 240.0f, 1.f, 1.f, 1.f, 1.0f, 1.0f },
    {   0.0f, 0.0f, 1.f, 1.f, 1.f, 0.0f, 0.0f },
    {   512.0f, 0.0f, 1.f, 1.f, 1.f, 1.0f, 0.0f },
    {   512.0f, 240.0f, 1.f, 1.f, 1.f, 1.0f, 1.0f }
};


void OnCharInputCallback(u32 c)
{
    ImGuiIO& io = ImGui::GetIO();
    io.AddInputCharacter(c);
}


void OnKeyCallback (int key, int action, int mods)
{
    ImGuiIO& io = ImGui::GetIO();
    if (action == 1) // press
        io.KeysDown[key] = true;
    if (action == 0)
        io.KeysDown[key] = false;

    io.KeyCtrl = io.KeysDown[341] || io.KeysDown[345];
    io.KeyShift = io.KeysDown[340] || io.KeysDown[344];
    io.KeyAlt = io.KeysDown[342] || io.KeysDown[346];
    io.KeySuper = io.KeysDown[343] || io.KeysDown[347];
}   

void SetCallbacks(Memory* memory)
{
    memory->OnCharInputCallback = &OnCharInputCallback;
    memory->OnKeyCallback = &OnKeyCallback;
}


GAME_INIT_GRAPHIC(game_init_graphic)
{
    graph_init(memory);
    
    SetCallbacks(memory);
    
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;

    ImGui_ImplOpenGL3_Init("#version 100");
    ImGui::StyleColorsDark();
}

bool show_another_window = true;

#include "external/imgui/imgui_demo.cpp"

GAME_LOOP(game_loop)
{
    ImGuiIO& io = ImGui::GetIO(); (void)io;
    io.DisplaySize.x = memory->screen_width;
    io.DisplaySize.y = memory->screen_height;

    io.MousePos.x = memory->input.mouse_x;
    io.MousePos.y = memory->input.mouse_y;

    io.MouseWheel = memory->input.mouse_sy;
    io.MouseWheelH = memory->input.mouse_sx;


    for (int i = 0; i < 5; i++)
    {
        io.MouseDown[i] =  memory->input.mouse_btn[i];
    } 

    ImGui_ImplOpenGL3_NewFrame();
    ImGui::NewFrame();

    graph_draw_all(memory);


    //printf("%5d : FUCK THE RULES ! !\n", memory->screen_width);
    memory->x ++ ;

    ImGui::BeginMainMenuBar();
        //ImGui::Text("%f05.3", memory->debug.lastFrameTimesMs[0]);
        if(ImGui::BeginMenu("Test"))
        {
            ImGui::PlotLines("FrameGraph", memory->debug.lastFrameTimesMs, DEBUG_FRAMES_COUNT, memory->debug.currentTimeFrame, 0, 0.0f, 100.0f, ImVec2(300,300));
            ImGui::EndMenu();
        }
    ImGui::EndMainMenuBar();

    if (show_another_window)
    {
    ImGui::ShowDemoWindow(&show_another_window);
    }

    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

}

GAME_UNLOAD_GRAPHIC(game_unload_graphic)
{
    ImGui_ImplOpenGL3_Shutdown();
    
    ImGui::DestroyContext();
    //stbi_image_free(test_image);
    // Do nothing at the moment
}