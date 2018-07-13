#pragma once
#include <cstdint>
typedef int32_t s32;
typedef int64_t s64;
typedef float   r32;
typedef double  r64;


struct Memory
{
    s32 x;
    s32 y;
    
    s32 screen_width;
    s32 screen_height;
    
    void* proc;
};

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