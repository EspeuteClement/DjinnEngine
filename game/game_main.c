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
str* _game_data = NULL;
void* game_data = NULL;

void init()
{
	_game_data = (str*) game_data;
	resource_load_spritesheet(spritesheets_01);
}

void step() {
	_game_data->x = (_game_data->x + 1) % SCREEN_W;
}

void draw()
{
	draw_quad(_game_data->x,_game_data->y,0,64,64,0,0,0,0);
}