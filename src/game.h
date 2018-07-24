#pragma once
#include "tools/djinn_pack_api.h"

#include <cstdint>
typedef int32_t s32;
typedef uint32_t u32;
typedef uint64_t u64;
typedef int64_t s64;
typedef float   r32;
typedef double  r64;


struct Input
{
    r32 mouse_x;
    r32 mouse_y;
    int mouse_btn[5];

    float mouse_sx;
    float mouse_sy;
};

#define DEBUG_FRAMES_COUNT 60
struct Debug
{
    r32 lastFrameTimesMs[DEBUG_FRAMES_COUNT];
    int currentTimeFrame;
};

inline void AddDebugFrame(Debug *debug, r32 lastTime)
{
    debug->lastFrameTimesMs[debug->currentTimeFrame] = lastTime;
    debug->currentTimeFrame = (debug->currentTimeFrame + 1) % DEBUG_FRAMES_COUNT;
}

#define DJN_TEXTURE_WIDTH (1024)
#define DJN_TEXTURE_SIZE (DJN_TEXTURE_WIDTH * DJN_TEXTURE_WIDTH * 4)
#define IMAGE_DATA_MAX (512)
struct Graph
{
    int img_width;
    int img_height;
    unsigned char texture[DJN_TEXTURE_SIZE];

    pack_data data[IMAGE_DATA_MAX];
};

struct Memory
{
    s32 x;
    s32 y;
    
    s32 screen_width;
    s32 screen_height;

    Graph graph;
    Input input;

    // These function are ment to be called from the main application when the envents are encountered
    void (*OnCharInputCallback) (u32 inputChar);
    void (*OnKeyCallback) (int key, int action, int mods);

    void* proc;

    Debug debug;
};

// Note : Never store this pointer !!! It can become invalid at any moment
extern Memory* djn_memory;


#ifdef __STANDALONE__
    #define GAME_API extern
#else
    #define GAME_API extern "C" __declspec(dllexport)
#endif

#define GAME_LOOP(name) void name(Memory* memory)
typedef GAME_LOOP(game_loop_handle);
GAME_API GAME_LOOP(game_loop);

#define GAME_INIT_GRAPHIC(name) void name(Memory* memory)
typedef GAME_INIT_GRAPHIC(game_init_graphic_handle);
GAME_API GAME_INIT_GRAPHIC(game_init_graphic);

#define GAME_UNLOAD_GRAPHIC(name) void name(Memory* memory)
typedef GAME_UNLOAD_GRAPHIC(game_unload_graphic_handle);
GAME_API GAME_UNLOAD_GRAPHIC(game_unload_graphic);