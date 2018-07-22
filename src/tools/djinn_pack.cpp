// Packs a bunch of images from a folder 

#include "djinn_pack_api.h"
#include "djinn_pack_api.cpp"

#include <windows.h>
#include <cstdint>
#include <cstring>
#include <cstdio>

#define STB_IMAGE_IMPLEMENTATION
#include "../external/stb_image.h"

#define STB_RECT_PACK_IMPLEMENTATION
#include "../external/stb_rect_pack.h"

#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "../external/stb_image_write.h"

struct Img
{
    char name[64 + 1];
    pack_quad q;
    int w,h;
    unsigned char * data;
};

// open path and then extract all the png in that folder. Return null once all the
// images have been extracted



#define PIXEL_SIZE 4
// Copy in to out at the given position, cropping using the given pack_quad
void cpy_image( const unsigned char* in, 
                int in_w,
                const pack_quad& q,
                unsigned char * out, 
                int out_x, 
                int out_y,
                int out_w, 
                int out_h)
{
    int q_width = q.u2 - q.u1 + 1;
    int q_height = q.v2 - q.v1 + 1;

    if ((q_width + out_x >= out_w) || (q_height + out_y >= out_h))
    {
        //printf("ERROR : Out of the image\n");
        return;
    }
    for (int source_y = q.v1; source_y < q.v2; source_y ++)
    {
        memcpy( &out[((out_y + source_y-q.v1) * out_w + out_x) * PIXEL_SIZE ], 
                &in[(source_y * in_w + q.u1) * PIXEL_SIZE], 
                q_width * PIXEL_SIZE);
    }

}

void find_quad(unsigned char* in_data, pack_quad& q, int w, int h)
{
    #define COORD(_x,_y) ((((_y) * w) + (_x)) * PIXEL_SIZE)
    bool found_v1 = false;
    bool found_v2 = false;
    bool found_u1 = false;
    bool found_u2 = false;

    for (int y = 0; y < h; y++)
    {
        for (int x = 0; x < w; x++)
        {
            if (!found_v1)
            {
                if (in_data[COORD(x,y) + 3] > 0)
                {
                    q.v1 = y;
                    found_v1 = true;
                }
            }

            if (!found_v2)
            {
                if (in_data[COORD(x,h-y-1) + 3] > 0)
                {
                    q.v2 = h - y;
                    found_v2 = true;
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
                    q.u1 = x;
                    found_u1 = true;
                }
            }

            if (!found_u2)
            {
                if (in_data[COORD(w - x - 1,y) + 3] > 0)
                {
                    q.u2 = w - x;
                    found_u2 = true;
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
int main(int argc, char * argv)
{

    int pad = 1;

    Img* imgs;
    imgs = (Img*) malloc(sizeof(Img) * IMG_MAX);
    uint32_t imgs_index = 0;
    memset(imgs, 0, sizeof(Img));

    char out_path[256];
    char out_data[256];

    sprintf(out_path, "../pack.png");
    sprintf(out_data,"../pack.dat");

    stbrp_rect imgs_rects[IMG_MAX];

    char buff[256];
    sprintf(buff, "*.png", "data");

    printf("Searching files %s ... \n", buff);
    WIN32_FIND_DATAA fileFindData;
    HANDLE hndl = INVALID_HANDLE_VALUE;
    hndl = FindFirstFileA(buff, &fileFindData);

    if (hndl == INVALID_HANDLE_VALUE )
    {
        printf("Couldn't init file search", fileFindData.cFileName);
        return -1;
    }

    do
    {
        if (fileFindData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
        {

        }
        else
        {

            // Load the data
            Img* img            = &imgs[imgs_index];
            stbrp_rect* rect    = &imgs_rects[imgs_index];

            strcpy_s(img->name, sizeof(img->name), fileFindData.cFileName);



            int w,h,stuff;
            img->data = stbi_load((const char *)fileFindData.cFileName, &w, &h, &stuff, 4);
            
            img->w = w;
            img->h = h;
            
            if (img->data)
            {
                printf(".");
                rect->id = imgs_index;

                img->q.u1 = 0;
                img->q.v1 = 0;
                img->q.u2 = img->w;
                img->q.v2 = img->h;

                find_quad(img->data, img->q, img->w, img->h);

                //printf("    %s %dx%d --- pack_quad %d %d %d %d\n", img->name, img->w, img->h, img->q.u1, img->q.v1, img->q.u2, img->q.v2);
                imgs_index ++;

                rect->w = img->q.u2 - img->q.u1 + 2 * pad;
                rect->h = img->q.v2 - img->q.v1 + 2 * pad;
            }
            else
            {
                //printf("Error: Couldn't parse %s\n", fileFindData.cFileName);
            }
        }

    }
    while(FindNextFileA(hndl, &fileFindData) != 0 && imgs_index < IMG_MAX);

    printf("\nDone, found %d files !", imgs_index);
    printf("\nStart Packing !\n");
    stbrp_node nodes[NODE_COUNT];
    stbrp_context ctxt;

    unsigned char * out_image_data = (unsigned char*) malloc(IMAGE_SIZE * IMAGE_SIZE * PIXEL_SIZE);
    memset(out_image_data, 0, IMAGE_SIZE * IMAGE_SIZE * PIXEL_SIZE);
    stbrp_init_target(&ctxt, IMAGE_SIZE, IMAGE_SIZE, nodes, NODE_COUNT);

    stbrp_pack_rects(&ctxt, imgs_rects, imgs_index);

    printf("Pack done\n");

    // Preparing the pack
    pack_final out_pack;
    pack_open(out_data, out_pack, "w");
    out_pack.num_images = imgs_index;
    out_pack.pack_data_buffer = (pack_data*) malloc(imgs_index * sizeof(pack_data));
    out_pack.pack_name_buffer = (pack_name*) malloc(imgs_index * sizeof(pack_name));

    for (int i = 0; i < imgs_index; i++)
    {
        
        stbrp_rect* rect    = &imgs_rects[i];
        Img* img        = &imgs[rect->id];
        
        if (rect->was_packed)
        {
            //printf(".");

            // Copy image onto destination
            cpy_image(img->data, 
                        img->w, 
                        img->q, 
                        out_image_data,
                        rect->x + pad, 
                        rect->y + pad, 
                        IMAGE_SIZE, 
                        IMAGE_SIZE);
            stbi_image_free(img->data);

            pack_data &data = out_pack.pack_data_buffer[i];
            data.q.u1 = rect->x;
            data.q.v1 = rect->y;
            data.q.u2 = rect->x + rect->w;
            data.q.v2 = rect->y + rect->h;

            data.ox = img->q.u1;
            data.oy = img->q.v1;
            data.ow = img->w;
            data.oh = img->h;

            pack_name &name = out_pack.pack_name_buffer[i];
            strcpy_s(name.name, img->name);
            name.id = i;

            //fprintf(out_data_file,"%04X%04X%04X%04X%04X%04X\n", data.x ,data.y, data.q.u1, data.q.v1, data.q.u2, data.q.v2);
            //fwrite(&data, sizeof(data), 1, out_data_file);
        }
    }
    pack_write(out_pack);
    pack_close(out_pack);

    free(out_pack.pack_data_buffer);
    free(out_pack.pack_name_buffer);

    free(imgs);

    // test part
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
                printf("%16s : x:%04d y:%04d w:%04d h:%04d u1:%04d v1:%04d u2:%04d v2:%04d\n", name.name, data.ox ,data.oy, data.ow, data.oh, data.q.u1, data.q.v1, data.q.u2, data.q.v2);
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


    printf("\nOutput done");

    stbi_write_png(out_path, IMAGE_SIZE, IMAGE_SIZE, 4, out_image_data, IMAGE_SIZE * PIXEL_SIZE);


    free(out_image_data);
    FindClose(hndl);
}