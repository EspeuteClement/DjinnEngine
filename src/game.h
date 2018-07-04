#pragma once
#include <cstdint>
typedef int32_t s32;
typedef int64_t s64;
typedef float   f32;
typedef double  f64;

struct Memory
{
    s32 x;
    s32 y;
    struct SDL_Window* window;
};

#define GAME_LOOP(name) void name(Memory* memory)
typedef GAME_LOOP(game_loop_handle);
extern "C" __declspec(dllexport) GAME_LOOP(game_loop);