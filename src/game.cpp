#include "game.h"
#include <cstdio>

#include "graph.cpp"
#include "external/imgui/imgui.h"
#include "external/imgui/imgui_impl_djinn.h"

#define DR_WAV_IMPLEMENTATION
#include "external/mini_al/dr_wav.h"

#define MINI_AL_IMPLEMENTATION
#include "external/mini_al/mini_al.h"

Memory* djn_memory = nullptr;

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

#include <cmath>

float volume = 1.0f;

mal_uint32 on_send_frames_to_device(mal_device* pDevice, mal_uint32 frameCount, void* pSamples)
{
    mal_decoder* pDecoder = (mal_decoder*)pDevice->pUserData;
    if (pDecoder == NULL) {
        return 0;
    }

    mal_uint32 samples_read = mal_decoder_read(pDecoder, frameCount, pSamples);

    float* pSamplesFloat = (float*) pSamples;
    for (int i = 0; i < (samples_read*2); i++)
    {
        pSamplesFloat[i] = pSamplesFloat[i] * volume;
    }

    return samples_read;
}



mal_decoder decoder;
mal_device_config config;
mal_device device;
mal_decoder_config cfg;


GAME_INIT_GRAPHIC(game_init_graphic)
{
    djn_memory = memory;
    djn_gfx_init(memory);

    cfg.format = mal_format_f32;
    mal_result result = mal_decoder_init_file("data/stars.wav", &cfg, &decoder);
    if (result != MAL_SUCCESS) {
        printf("Couldn't init MAL file\n");
    }


    config = mal_device_config_init_playback(
        decoder.outputFormat,
        decoder.outputChannels,
        decoder.outputSampleRate,
        on_send_frames_to_device);
    
    
    if (mal_device_init(NULL, mal_device_type_playback, NULL, &config, &decoder, &device) != MAL_SUCCESS) {
        printf("Failed to open playback device.\n");
        mal_decoder_uninit(&decoder);
    }

    if (mal_device_start(&device) != MAL_SUCCESS) {
        printf("Failed to start playback device.\n");
        mal_device_uninit(&device);
        mal_decoder_uninit(&decoder);
    }

    SetCallbacks(memory);
    
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;

    ImGui_ImplOpenGL3_Init("#version 100");
    ImGui::StyleColorsDark();
}

void djn_game_debug_menu(Memory* memory)
{
    float mean_fps = 0.0f;
    float min = 100;
    float max = 0;
    for (int i = 0; i < DEBUG_FRAMES_COUNT; i++)
    {
        float value = memory->debug.lastFrameTimesMs[i]; 
        mean_fps += value / (float) DEBUG_FRAMES_COUNT;
        min = min > value ? value : min;
        max = max < value ? value : max;
    }
    ImGui::BeginMainMenuBar();
        ImGui::Text("fps : %03.1f",  1000.0f/(mean_fps));
        if(ImGui::IsItemHovered())
        {
            ImGui::BeginTooltip();
            ImGui::PlotLines("FrameGraph", memory->debug.lastFrameTimesMs, DEBUG_FRAMES_COUNT, memory->debug.currentTimeFrame, 0, 0.0f, 40.0f, ImVec2(300,150));
            ImGui::Text("min: %04.2f max: %04.2f avg:%04.2f", min, max, mean_fps);
            ImGui::EndTooltip();
        }
        ImGui::Text("Hello world");

    ImGui::EndMainMenuBar();

    ImGui::SliderFloat("Volume", &volume, 0.0f, 1.0f);
    ImGui::Text("Samples : %d", device.)
}

void djn_game_imgui_begin()
{
    Memory* memory = djn_memory;
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
}

void djn_game_imgui_end()
{
    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

GAME_LOOP(game_loop)
{
    djn_memory = memory;
    djn_game_imgui_begin();

    djn_gfx_draw_all(memory);
    djn_game_debug_menu(memory);

    djn_game_imgui_end();
}

GAME_UNLOAD_GRAPHIC(game_unload_graphic)
{
    ImGui_ImplOpenGL3_Shutdown();
    
    ImGui::DestroyContext();

    mal_device_uninit(&device);
    mal_decoder_uninit(&decoder);
    //stbi_image_free(test_image);
    // Do nothing at the moment
}