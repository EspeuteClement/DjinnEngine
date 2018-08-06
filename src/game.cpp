#include "game.h"
#include <cstdio>

#include "audio.h"
#include "graph.h"

#include "external/imgui/imgui.h"
#include "external/imgui/imgui_impl_djinn.h"



GameData* djn_game_data = nullptr;

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

void SetCallbacks(GameData* game_data)
{
    game_data->OnCharInputCallback = &OnCharInputCallback;
    game_data->OnKeyCallback = &OnKeyCallback;
}

#include <cmath>




void djn_game_imgui_begin()
{
    GameMemory* mem = djn_memory();
    ImGuiIO& io = ImGui::GetIO(); (void)io;
    io.DisplaySize.x = djn_game_data->screen_width;
    io.DisplaySize.y = djn_game_data->screen_height;

    io.MousePos.x = mem->input.mouse_x;
    io.MousePos.y = mem->input.mouse_y;

    io.MouseWheel = mem->input.mouse_sy;
    io.MouseWheelH = mem->input.mouse_sx;


    for (int i = 0; i < 5; i++)
    {
        io.MouseDown[i] =  mem->input.mouse_btn[i];
    } 

    ImGui_ImplOpenGL3_NewFrame();
    ImGui::NewFrame();
}

void djn_game_imgui_end()
{
    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

djn_audio_sound test_sound;

GAME_INIT(game_init)
{
    printf(" --- Init game --- \n");
    if (game_data->memory_size != sizeof(GameData))
    {
        return djnSta_ERROR_MEMORY;
    }

    djn_game_data = game_data;
    djn_gfx_init(game_data);

    ImGui::CreateContext();
    ImGui_ImplOpenGL3_Init("#version 100");
    ImGui::StyleColorsDark();

    djn_audio_init();
    SetCallbacks(game_data);

    test_sound = djn_audio_load_sound("data/coin.wav");

    return djnSta_OK;
}


void djn_game_menu_bar(GameData* game_data)
{
    float mean_fps = 0.0f;
    float min = 100;
    float max = 0;
    for (int i = 0; i < DEBUG_FRAMES_COUNT; i++)
    {
        float value = game_data->debug.lastFrameTimesMs[i]; 
        mean_fps += value / (float) DEBUG_FRAMES_COUNT;
        min = min > value ? value : min;
        max = max < value ? value : max;
    }

    ImGui::BeginMainMenuBar();
        ImGui::Text("fps : %03.1f",  1000.0f/(mean_fps));
        if(ImGui::IsItemHovered())
        {
            ImGui::BeginTooltip();
            ImGui::PlotLines("FrameGraph", game_data->debug.lastFrameTimesMs, DEBUG_FRAMES_COUNT, game_data->debug.currentTimeFrame, 0, 0.0f, 40.0f, ImVec2(300,150));
            ImGui::Text("min: %04.2f max: %04.2f avg:%04.2f", min, max, mean_fps);
            ImGui::EndTooltip();
        }
        
        if(ImGui::BeginMenu("Debug"))
        {
            const char * items[] =
            {
                "GDM_Normal",
                "GDM_Window"
            };

            ImGui::Combo("Game Draw Mode", (int*) &(game_data->debug.game_draw_mode), items, 2, -1);
            ImGui::EndMenu();
        }

    ImGui::EndMainMenuBar();

    if (ImGui::Button("Play test sound"))
    {
        djn_audio_play_sound(test_sound);
    }

    if (ImGui::Button("Play Music"))
    {
        djn_audio_music_play("data/calm.ogg");
    }

    ImGui::SameLine();

    if (ImGui::Button("Stop Music"))
    {
        djn_audio_music_stop();
    }

    ImGui::Text("Samples : %d", djn_game_data->debug.lastTimeSamplesRead);
}

void djn_game_debug_menu(GameData* game_data)
{
    djn_game_menu_bar(game_data);

    GameMemory* mem = djn_memory();

    ImGui::Begin("Inputs");
    ImGui::Text("%d", mem->input.currentHistoryFrame);

    ImGui::Text("U L D R");
    for (int i = 0; i < MAX_INPUT_HISTORY; ++i)
    {
        int id = (mem->input.currentHistoryFrame - i + MAX_INPUT_HISTORY) % MAX_INPUT_HISTORY;
        u32 key = mem->input.inputs[0][id].Keyflags;
        ImGui::Text("%d %d %d %d"   , (key & djnKey::djnKey_UP) != 0
                                    , (key & djnKey::djnKey_LEFT) != 0
                                    , (key & djnKey::djnKey_DOWN) != 0
                                    , (key & djnKey::djnKey_RIGHT) != 0);
    }

    ImGui::End();

    /*ImGui::SliderFloat("Volume", &volume, 0.0f, 1.0f);
    ImGui::InputInt("Freq", &pitch_index, 1, 12);
    pitch_index %= NUM_NOTES;
    ImGui::Text("Samples : %d", stb_vorbis_get_sample_offset(vorbis_file));
    ImGui::Checkbox("System Pause", &(game_data->is_system_paused));*/
}

bool djn_key(u8 player, djnKey key)
{
    return djn_game_data->djn_memory.input.key(player, key);
}

GAME_LOOP(game_loop)
{
    if (game_data->is_system_paused)
        return;
    djn_game_data = game_data;
    djn_game_imgui_begin();

    GameMemory* mem = djn_memory();
    int speed = 4;
    if (djn_key(0, djnKey_RIGHT))
    {
        mem->x += speed;    
    }
    if (djn_key(0, djnKey_LEFT))
    {
        mem->x -= speed;    
    }

    if (djn_key(0, djnKey_UP))
    {
        mem->y -= speed;    
    }
    if (djn_key(0, djnKey_DOWN))
    {
        mem->y += speed;
        djn_audio_play_sound(test_sound);
    }

    djn_gfx_draw_all(game_data);
    djn_game_debug_menu(game_data);

    djn_game_imgui_end();
}

GAME_DEINIT(game_deinit)
{
    printf(" --- Deinit game --- \n");

    djn_audio_deinit();

    ImGui_ImplOpenGL3_Shutdown();
    ImGui::DestroyContext();
    djn_gfx_deinit(game_data);
    //stbi_image_free(test_image);
    // Do nothing at the moment
}