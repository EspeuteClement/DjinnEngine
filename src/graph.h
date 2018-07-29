// Djinn Engine Graphics Library
//
#pragma once

// Includes
#include <cstdint>

struct GameData;

// Init all the graphics
void djn_gfx_init(GameData* mem);

// Debug function that draw stuff on the screen
void djn_gfx_draw_all(GameData* mem);

void djn_gfx_deinit(GameData* game_data);

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

// Graphics ==================================================================
struct djn_quad
{
    vec2u16 offset;
    vec2u16 originalSize;
    vec2u16 quadSize;
    vec2f uv[2];
};

#define DJN_TEXTURE_WIDTH (1024)
#define DJN_TEXTURE_SIZE (DJN_TEXTURE_WIDTH * DJN_TEXTURE_WIDTH * 4)
#define IMAGE_DATA_MAX (512)
struct Graph
{
    int img_width;
    int img_height;
    unsigned char texture[DJN_TEXTURE_SIZE];

    djn_quad data[IMAGE_DATA_MAX];
};



