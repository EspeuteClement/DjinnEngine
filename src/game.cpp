#include "game.h"
#include <cstdio>

#include "external/imgui/imgui.h"
#include "external/imgui/imgui_impl_djinn.h"

//#define DR_WAV_IMPLEMENTATION
//#include "external/mini_al/dr_wav.h"

#define MINI_AL_IMPLEMENTATION
#include "external/mini_al/mini_al.h"

// OGG playback
#define STB_VORBIS_HEADER_ONLY
#include "external/stb_vorbis.c"
#undef L
#undef R
#undef C


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

float volume = 1.0f;



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

mal_device_config config;
mal_device device;
mal_context Ctxt;

uint32_t sample_pos = 0;
int32_t pitch_index = 25;
#define SAMPLE_RATE 44100

#define NUM_NOTES 88
static int pitch[NUM_NOTES];


float samples[]
=
{
    1.0f,
    1.0f,
    1.0f,
    1.0f,
    1.0f,
    1.0f,
    1.0f,
    1.0f,
    1.0f,
    1.0f,
    1.0f,
    1.0f,
    1.0f,
    1.0f,
    1.0f,
    1.0f,
    -1.0f,
    -1.0f,
    -1.0f,
    -1.0f,
    -1.0f,
    -1.0f,
    -1.0f,
    -1.0f,
    -1.0f,
    -1.0f,
    -1.0f,
    -1.0f,
    -1.0f,
    -1.0f,
    -1.0f,
    -1.0f
};

float sample_acc = 0.0f;

static double GetNoteSampleRate(int note)
{
	return (pitch[note]) / 44100.0f;
}

static double interpolate(float a, float b, float c)
{
    return a + (b-a) * c;
}

stb_vorbis* vorbis_file;

// Overkill optim : Do that with smid ?
void scale_buffer(float* in_buffer, float volume, u32 nb_floats)
{
    for (u32 cursor = 0; cursor < nb_floats; ++ cursor)
    {
        in_buffer[cursor] *= volume;
    }
}

bool should_play = false;
mal_uint32 on_send_frames_to_device(mal_device* pDevice, mal_uint32 frameCount, void* pSamples)
{
    /*mal_decoder* pDecoder = (mal_decoder*)pDevice->pUserData;
    if (pDecoder == NULL) {
        return 0;
    }

    mal_uint32 samples_read = mal_decoder_read(pDecoder, frameCount, pSamples);

    float* pSamplesFloat = (float*) pSamples;
    for (int i = 0; i < (samples_read*2); i++)
    {
        pSamplesFloat[i] = pSamplesFloat[i] * volume;
    }

    return samples_read;*/
#if 0
    float sample_speed = GetNoteSampleRate(pitch_index) * 32;

    float* pSamplesFloat = (float*) pSamples;
    for (int i = 0; i < (frameCount*2); i++)
    {
        int current_sample = ((int)sample_acc) % 32;
        int sample_2 = (current_sample + 1) % 32;
        float vol = volume * 0.1f * interpolate(samples[current_sample], samples[sample_2], sample_acc - (int) sample_acc);
        
		pSamplesFloat[i] = vol;

        if ((i % 2) == 1)
        {
            sample_acc += sample_speed;
            if (sample_acc > 32)
            {
                sample_acc -= 32;
            }
        }


    }
#endif
    if (djn_game_data->is_system_paused || !should_play)
        return 0;
    u32 samples_read = stb_vorbis_get_samples_float_interleaved(vorbis_file, 2, (float*) pSamples, frameCount * 2);
    
    if (samples_read < frameCount)
    {
        stb_vorbis_seek(vorbis_file, 0);
        samples_read += stb_vorbis_get_samples_float_interleaved(vorbis_file, 2, &((float*)pSamples)[samples_read * 2], (frameCount-samples_read) * 2);
    }

    scale_buffer((float*) pSamples, volume, frameCount * 2);

    // Mix

    return frameCount;
}



static void init_pitch(void)
{
    
    for(int i=0;i<NUM_NOTES;i++)
    {
        pitch[i] = (int)(441.0*(pow(2.0,((i-19.0)/12.0)))); // https://en.wikipedia.org/wiki/Equal_temperament
    }
}

void init_audio()
{
    /*cfg.format = mal_format_f32;
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
    }*/

    int error = 0;
    vorbis_file = stb_vorbis_open_filename("data/calm.ogg", &error, NULL);

    if (error)
    {
        printf("Error Couldn open vorbis file\n");
        return;
    }
    else
    {
        printf("Vorbis file opened\n");
    }



    init_pitch();

    if (mal_context_init(NULL, 0, NULL, &Ctxt) != MAL_SUCCESS)
    {
        printf("Couldn't init audio context\n");
        return;
    }

    config = mal_device_config_init(mal_format_f32, 2, SAMPLE_RATE, NULL, &on_send_frames_to_device);

    if (mal_device_init(&Ctxt, mal_device_type_playback,NULL, &config, NULL, &device))
    {
        printf("Couldn't init mal device\n");
        mal_context_uninit(&Ctxt);
        return;
    }

    if (mal_device_start(&device) != MAL_SUCCESS)
    {
        printf("Couldn't start mal device\n");
        mal_device_uninit(&device);
        mal_context_uninit(&Ctxt);
        return;
    }

    should_play = true;
}

void deinit_audio()
{
    should_play = false;
    stb_vorbis_close(vorbis_file);
    mal_device_uninit(&device);
}

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

    init_audio();
    SetCallbacks(game_data);

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

    ImGui::SliderFloat("Volume", &volume, 0.0f, 1.0f);
    ImGui::InputInt("Freq", &pitch_index, 1, 12);
    pitch_index %= NUM_NOTES;
    ImGui::Text("Samples : %d", stb_vorbis_get_sample_offset(vorbis_file));
    ImGui::Checkbox("System Pause", &(game_data->is_system_paused));
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
    }


    djn_gfx_draw_all(game_data);
    djn_game_debug_menu(game_data);

    djn_game_imgui_end();
}

GAME_DEINIT(game_deinit)
{
    printf(" --- Deinit game --- \n");

    deinit_audio();

    ImGui_ImplOpenGL3_Shutdown();
    ImGui::DestroyContext();
    djn_gfx_deinit(game_data);
    //stbi_image_free(test_image);
    // Do nothing at the moment
}