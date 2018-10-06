#ifndef _DJN_RESOURCES_H_
#define _DJN_RESOURCES_H_

#include "ugl/ugl.h"

#include "stdint.h"

typedef uint32_t spritesheet_id_t;
typedef uint32_t sprite_id_t;
typedef uint8_t* texture_t; 
typedef GLuint 	gpu_texture_t;

typedef struct
{
	const char* path;
	const uint16_t width;
	const uint16_t height;
} spritesheet_data;

typedef struct
{
	float u0;
	float v0;
	float v1;
	float u1;
} uv;

typedef struct
{
	const spritesheet_id_t spritesheet_id;
	const uv uvs; 
} sprite_data;

typedef struct
{
	texture_t pointer;
	gpu_texture_t gl_texture;
} spritesheet_data_storage;

#define MAX_TEXTURES 256
extern spritesheet_data_storage storage[MAX_TEXTURES];
extern int game_current_texture_count;
extern const spritesheet_data* game_current_spritesheet_data;

// Resource api
gpu_texture_t 	resource_load_spritesheet(spritesheet_id_t id); 	// Load resource into memory. Returns null if the texture doesn't exist
int 		resource_load_sprite(sprite_id_t id);							// Load the spritesheet where this sprite resides, and return pointer to the raw texture data
void 		resource_free_spritesheets();						// Free all spritesheets
void resource_free_spritesheet(spritesheet_id_t id);


// Drawing api
void	draw_sprite(sprite_id_t id, int32_t x, int32_t y);		// Draw the sprite, load the sprite if it is not loaded. Errors if that sprite doesn't exist



#endif