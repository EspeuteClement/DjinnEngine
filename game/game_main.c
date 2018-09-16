#include <stdio.h>
#include <stdint.h>
#include "djn_graph.h"
#include "djn_resources.h"
#include "game_resources.h"

typedef struct
{
  int x;
  int y;
} str;

const size_t game_data_size = sizeof(str);
str* game_data = NULL;

void init()
{
	resource_load_spritesheet(spritesheets_01);
}

void step() {
	game_data->x = (game_data->x + 1) % SCREEN_W;
}

void draw()
{
	draw_quad(game_data->x,game_data->y,0,64,64,0,0,0,0);
}