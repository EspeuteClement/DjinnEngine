#undef STB_IMAGE_IMPLEMENTATION
#include "external/stb_image.h"

#define STB_RECT_PACK_IMPLEMENTATION
#include "external/stb_rect_pack.h"

#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "external/stb_image_write.h"

#define CUTE_FILES_IMPLEMENTATION
#include "external/cute_files.h"
#undef CUTE_FILES_IMPLEMENTATION

#include <stdint.h>

#include "djn_alloc.h"
#include "djn_files.h"

// Constant

static const char* output_file_pattern = "test_texture_%02d.png"; 


typedef struct 
{
    uint16_t u1, v1, u2, v2;
} uv_t;


typedef struct
{
    char name[64 + 1];
    uv_t source_quad;
    int w,h;
    unsigned char * data;
} Img;

#define PIXEL_SIZE 4
// Copy in to out at the given position, cropping using the given uv_t
void cpy_image( const unsigned char* in, 
                int in_w,	// Width of the source image
                const uv_t* source_quad,
                unsigned char * out, 
                int out_x, 
                int out_y,
                int out_w, 
                int out_h)
{
    int q_width = source_quad->u2 - source_quad->u1 + 1;
    int q_height = source_quad->v2 - source_quad->v1 + 1;

    if ((q_width + out_x >= out_w) || (q_height + out_y >= out_h))
    {
        //printf("ERROR : Out of the image\n");
        return;
    }
    for (int source_y = source_quad->v1; source_y < source_quad->v2; source_y ++)
    {
        memcpy( &out[((out_y + source_y-source_quad->v1) * out_w + out_x) * PIXEL_SIZE ], 
                &in[(source_y * in_w + source_quad->u1) * PIXEL_SIZE], 
                q_width * PIXEL_SIZE);
    }
}

void find_quad(unsigned char* in_data, uv_t* source_quad, int w, int h)
{
    #define COORD(_x,_y) ((((_y) * w) + (_x)) * PIXEL_SIZE)
    uint8_t found_v1 = 0;
    uint8_t found_v2 = 0;
    uint8_t found_u1 = 0;
    uint8_t found_u2 = 0;

    for (int y = 0; y < h; y++)
    {
        for (int x = 0; x < w; x++)
        {
            if (!found_v1)
            {
                if (in_data[COORD(x,y) + 3] > 0)
                {
                    source_quad->v1 = y;
                    found_v1 = 1;
                }
            }

            if (!found_v2)
            {
                if (in_data[COORD(x,h-y-1) + 3] > 0)
                {
                    source_quad->v2 = h - y;
                    found_v2 = 1;
                }
            }

            if (found_v1 && found_v2)
            {
                break;
            }
        }
    }

    for (int x = 0; x < w; x++)
    {
        for (int y = 0; y < h; y++)
        {
            if (!found_u1)
            {
                if (in_data[COORD(x,y) + 3] > 0)
                {
                    source_quad->u1 = x;
                    found_u1 = 1;
                }
            }

            if (!found_u2)
            {
                if (in_data[COORD(w - x - 1,y) + 3] > 0)
                {
                    source_quad->u2 = w - x;
                    found_u2 = 1;
                }
            }

            if (found_u1 && found_u2)
            {
                break;
            }
        }
    }
    #undef COORD
}


#define IMG_MAX 1000
#define NODE_COUNT IMG_MAX
#define IMAGE_SIZE 1024
void djn_sprite_batch_dir(const char * path, int force_batch)
{
    int pad = 1;

    Img* imgs;
    imgs = (Img*) djn_alloc(sizeof(Img) * IMG_MAX);
    uint32_t imgs_index = 0;
    memset(imgs, 0, sizeof(Img));

    char out_path[256];
    char out_data[256];

    sprintf(out_path, "test_pack.png");
    sprintf(out_data,"../pack.dat");

    stbrp_rect imgs_rects[IMG_MAX] = {0};
    stbrp_rect imgs_rects_transfer[IMG_MAX] = {0};

    char buff[256];
    sprintf(buff, "*.png");

    printf("Searching files in %s ... \n", path);

    djn_timestamp_t oldest_atlas = -1;

    // Find oldest_atlas
    {
        int atlas_index = 0;
        char atlas_names[64];
        
		for (atlas_index = 0; atlas_index < 99; atlas_index++)
		{
			sprintf(atlas_names, output_file_pattern, atlas_index);

			djn_timestamp_t current_atlas_date = djn_files_last_edit_time(atlas_names);

			if (current_atlas_date == 0)
				break;

			if (current_atlas_date < oldest_atlas)
				oldest_atlas = current_atlas_date;

		}
    }

	int is_newer = force_batch;
    {
        cf_dir_t dir;
        cf_dir_open(&dir, path);

        // Find newest sprite
        while(dir.has_next && imgs_index < IMG_MAX)
        {
            cf_file_t file;
            cf_read_file(&dir, &file);

            if (!file.is_dir)
            {
                djn_timestamp_t current_sprite_date = djn_files_last_edit_time(file.path);
                if (current_sprite_date > oldest_atlas)
                {
                    is_newer = 1;
                    break;
                }
            }

            cf_dir_next(&dir);
        }

        cf_dir_close(&dir);
    }

	if (is_newer != 0)
	{
		cf_dir_t dir;
		cf_dir_open(&dir, path);

		while (dir.has_next && imgs_index < IMG_MAX)
		{
			cf_file_t file;
			cf_read_file(&dir, &file);

			//fprintf(stdout, "%s : %lld\n", file.path, djn_files_last_edit_time(file.path));
			//fprintf(stdout, "%s : %lld\n", file.path, djn_files_last_edit_time(file.path));

			if (!file.is_dir)
			{
				if (file.ext[1] == 'p' &&
					file.ext[2] == 'n' &&
					file.ext[3] == 'g' &&
					file.ext[4] == '\0'
					)
				{
					// Load the data
					Img* img = &imgs[imgs_index];
					stbrp_rect* rect = &imgs_rects[imgs_index];

					int w, h, stuff;
					img->data = stbi_load((const char *)file.path, &w, &h, &stuff, 4);

					img->w = w;
					img->h = h;

					if (img->data)
					{
						//printf(".");
						rect->id = imgs_index;

						img->source_quad.u1 = 0;
						img->source_quad.v1 = 0;
						img->source_quad.u2 = img->w;
						img->source_quad.v2 = img->h;

						find_quad(img->data, &(img->source_quad), img->w, img->h);

						imgs_index++;

						rect->w = img->source_quad.u2 - img->source_quad.u1 + 2 * pad;
						rect->h = img->source_quad.v2 - img->source_quad.v1 + 2 * pad;

						// copy name
						int cursor = 0;
						while (file.name[cursor] != '0' && file.name[cursor] != '.')
						{
							char current_char = file.name[cursor];
							switch (current_char)
							{
							case ' ':
							case '(':
							case ')':
							case '-':
								current_char = '_';
								break;
							default:
								break;
							}

							img->name[cursor] = current_char;

							cursor++;
						}
						img->name[cursor] = 0;
					}
					else
					{
						fprintf(stderr, "Error: Couldn't parse %s\n", file.path);
					}
				}
			}

			cf_dir_next(&dir);
		}

		cf_dir_close(&dir);

		printf("\nDone, found %d files !", imgs_index);
		printf("\nStart Packing !\n");
		stbrp_node nodes[NODE_COUNT];
		stbrp_context ctxt;



		int has_finished = 0;
		int current_texture_id = 0;
		do
		{
			unsigned char * out_image_data = (unsigned char*)djn_alloc(IMAGE_SIZE * IMAGE_SIZE * PIXEL_SIZE);
			memset(out_image_data, 0, IMAGE_SIZE * IMAGE_SIZE * PIXEL_SIZE);

			char output_file_name[64];
			sprintf(output_file_name, output_file_pattern, current_texture_id);

			stbrp_init_target(&ctxt, IMAGE_SIZE, IMAGE_SIZE, nodes, NODE_COUNT);
			has_finished = stbrp_pack_rects(&ctxt, imgs_rects, imgs_index);

			printf("Pack done\n");
			// Preparing the pack
			/*pack_final out_pack;
			pack_open(out_data, out_pack, "w");
			out_pack.num_images = imgs_index;
			out_pack.pack_data_buffer = (pack_data*) malloc(imgs_index * sizeof(pack_data));
			out_pack.pack_name_buffer = (pack_name*) malloc(imgs_index * sizeof(pack_name));*/

			int imgs_rects_transfer_index = 0;

			for (int i = 0; i < imgs_index; i++)
			{

				stbrp_rect* rect = &imgs_rects[i];
				Img* img = &imgs[rect->id];

				if (rect->was_packed)
				{
					printf(".");

					// Copy image onto destination
					cpy_image(img->data,
						img->w,
						&(img->source_quad),
						out_image_data,
						rect->x + pad,
						rect->y + pad,
						IMAGE_SIZE,
						IMAGE_SIZE);
					stbi_image_free(img->data);

					//printf("\t%s = %d,\n", img->name, rect->id);

					/*pack_data &data = out_pack.pack_data_buffer[i];
					data.source_quad.u1 = rect->x;
					data.source_quad.v1 = rect->y;
					data.source_quad.u2 = rect->x + rect->w;
					data.source_quad.v2 = rect->y + rect->h;

					data.ox = img->source_quad.u1;
					data.oy = img->source_quad.v1;
					data.ow = img->w;
					data.oh = img->h;

					pack_name &name = out_pack.pack_name_buffer[i];
					strcpy_s(name.name, img->name);
					name.id = i;*/

					//fprintf(out_data_file,"%04X%04X%04X%04X%04X%04X\n", data.x ,data.y, data.source_quad.u1, data.source_quad.v1, data.source_quad.u2, data.source_quad.v2);
					//fwrite(&data, sizeof(data), 1, out_data_file);
				}
				else
				{
					// Transfer to next block
					memcpy(&(imgs_rects_transfer[imgs_rects_transfer_index]), rect, sizeof(stbrp_rect));
					imgs_rects_transfer_index++;
				}
			}

			printf("\nOutput done");
			stbi_write_png(output_file_name, IMAGE_SIZE, IMAGE_SIZE, 4, out_image_data, IMAGE_SIZE * PIXEL_SIZE);
			djn_free(out_image_data);

			// if we still have image to pack, copy them
			if (!has_finished)
			{
				memcpy(imgs_rects, imgs_rects_transfer, sizeof(stbrp_rect) * imgs_rects_transfer_index);
				imgs_index = imgs_rects_transfer_index;

				current_texture_id++;
			}
		} while (!has_finished);

	}
	else
	{
		printf("No new sprites, skipping packing phase ...\n");
	}




    
    //pack_write(out_pack);
    //pack_close(out_pack);

    //free(out_pack.pack_data_buffer);
    //free(out_pack.pack_name_buffer);

    djn_free(imgs);

    // test part
    #if 0
    {
        pack_final in_pack;
        if(pack_open(out_data, in_pack, "r"))
        {
            in_pack.pack_data_buffer = (pack_data*) malloc(in_pack.num_images * sizeof(pack_data));
            in_pack.pack_name_buffer = (pack_name*) malloc(in_pack.num_images * sizeof(pack_name));

            pack_read(in_pack);
            printf("Images found : %d\n", in_pack.num_images);
            for(int i = 0; i < in_pack.num_images; i++)
            {
                pack_data& data = in_pack.pack_data_buffer[i];
                pack_name& name = in_pack.pack_name_buffer[i];
                printf("%16s : x:%04d y:%04d w:%04d h:%04d u1:%04d v1:%04d u2:%04d v2:%04d\n", name.name, data.ox ,data.oy, data.ow, data.oh, data.source_quad.u1, data.source_quad.v1, data.source_quad.u2, data.source_quad.v2);
            }

                    // Try to find some ids :

            auto test_func = [](const char * name, pack_name* db, size_t size)
            {
                int id = pack_find(name, db, size);
                if (id != -1)
                {
                    printf("Found %s : %d\n", name, id);
                }
                else
                {
                    printf("Couldn't find %s\n", name);
                }
            };

            test_func("Hector99.png", in_pack.pack_name_buffer, in_pack.num_images);
            test_func("zerztrsd", in_pack.pack_name_buffer, in_pack.num_images);
            test_func("test_src9.png", in_pack.pack_name_buffer, in_pack.num_images);
            test_func("Hector55.png", in_pack.pack_name_buffer, in_pack.num_images);
            test_func("Hector19.png", in_pack.pack_name_buffer, in_pack.num_images);

            pack_close(in_pack);
        }
    }
    #endif
}

