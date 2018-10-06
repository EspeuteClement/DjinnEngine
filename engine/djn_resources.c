#include "djn_resources.h"
#include "djn_assert.h"
#include "djn_alloc.h"

#define STBI_NO_SIMD
#define STBI_NO_HDR

#define STBI_MALLOC djn_alloc
#define STBI_FREE djn_free
#define STBI_REALLOC djn_realloc

#define STB_IMAGE_IMPLEMENTATION
#include "external/stb_image.h"
#include "djn_debug.h"

spritesheet_data_storage storage[MAX_TEXTURES] = {0};
const spritesheet_data* game_current_spritesheet_data = 0;


gpu_texture_t 	resource_load_spritesheet(spritesheet_id_t id)
{
	DJN_ASSERT(id >= 0 && id < game_current_texture_count);
	spritesheet_data_storage* store = &storage[id];

	if (store->gl_texture == 0)
	{
		int x,y,n;
		store->pointer = (texture_t)stbi_load(game_current_spritesheet_data[id].path, &x, &y, &n, 4);

		DJN_ASSERT(store->pointer);
		glGenTextures(1, &store->gl_texture);
		glBindTexture(GL_TEXTURE_2D, store->gl_texture);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, x, y, 0, GL_RGBA, GL_UNSIGNED_BYTE, store->pointer);

		printf("Texture (%s) loaded, glTexId : %d \n", game_current_spritesheet_data[id].path, store->gl_texture);

	}
	return store->gl_texture;
}

void resource_free_spritesheets()
{
	int i = 0;
	while(storage[i].pointer != NULL && i < MAX_TEXTURES)
	{
		resource_free_spritesheet(i);
		i ++;
	}
}

void resource_free_spritesheet(spritesheet_id_t id)
{
	DJN_ASSERT(id >= 0 && id < game_current_texture_count);

	if (storage[id].pointer)
	{
		stbi_image_free(storage[id].pointer);
		glDeleteTextures(1,&storage[id].gl_texture);

		storage[id].pointer = NULL;
		storage[id].gl_texture = 0;
	}
}

int game_current_texture_count = 0;
