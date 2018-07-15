#include "game.h"
#include <cstdio>

#include "graph.cpp"
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









GAME_INIT_GRAPHIC(game_init_graphic)
{
    graph_init(memory);
}


GAME_LOOP(game_loop)
{
    graph_draw_all(memory);


    //printf("%5d : FUCK THE RULES ! !\n", memory->screen_width);
    memory->x ++ ;
}

GAME_UNLOAD_GRAPHIC(game_unload_graphic)
{
    //stbi_image_free(test_image);
    // Do nothing at the moment
}