// Djinn Engine Graphics Library
//
#pragma once

// Includes
#include <cstdint>

struct Memory;

// Init all the graphics
void djn_gfx_init(Memory* mem);

// Debug function that draw stuff on the screen
void djn_gfx_draw_all(Memory* mem);

template <typename T>
struct djn_vec2
{
    union
    {
        T v[2];
        struct {T x,y;};
    };
};

typedef djn_vec2<float> vec2f;
typedef djn_vec2<int32_t> vec2i32;
typedef djn_vec2<uint16_t> vec2u16;


struct djn_quad
{
    vec2u16 offset;
    vec2u16 originalSize;
    vec2f uv[2];
    vec2f uvSize;
};