#ifndef _GAME_RESOURCES_H_
#define _GAME_RESOURCES_H_

#include "djn_resources.h"

enum spritesheets
{
	spritesheets_01,
	spritesheets_02,
	spritesheets_count,
};

extern const spritesheet_data spritesheets_paths[spritesheets_count];
extern const int game_spritesheets_count; 

#endif