// Packs a bunch of images from a folder 

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

struct quad
{
    int u1, v1, u2, v2;
};

struct Img
{
    char name[64 + 1];
    quad q;
    int w,h;
    unsigned char * data;
};

// open path and then extract all the png in that folder. Return null once all the
// images have been extracted



#define PIXEL_SIZE 4
void cpy_image( const unsigned char* in, 
                int in_w,
                const quad& q,
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

void find_quad(unsigned char* in_data, quad& q, int w, int h)
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

    stbrp_rect imgs_rects[IMG_MAX];


    char buff[256];
    sprintf(buff, "*.png", "data");

    //printf("Searching files %s ... \n\n", buff);
    WIN32_FIND_DATAA fileFindData;
    HANDLE hndl = INVALID_HANDLE_VALUE;
    hndl = FindFirstFileA(buff, &fileFindData);

    if (hndl == INVALID_HANDLE_VALUE )
    {
        //printf("Couldn't init file search", fileFindData.cFileName);
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
                
                rect->id = imgs_index;

                img->q.u1 = 0;
                img->q.v1 = 0;
                img->q.u2 = img->w;
                img->q.v2 = img->h;

                find_quad(img->data, img->q, img->w, img->h);

                //printf("    %s %dx%d --- quad %d %d %d %d\n", img->name, img->w, img->h, img->q.u1, img->q.v1, img->q.u2, img->q.v2);
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

    //printf("\n\n");

    stbrp_node nodes[NODE_COUNT];
    stbrp_context ctxt;

    unsigned char * out_image_data = (unsigned char*) malloc(IMAGE_SIZE * IMAGE_SIZE * PIXEL_SIZE);
    memset(out_image_data, 0, IMAGE_SIZE * IMAGE_SIZE * PIXEL_SIZE);
    stbrp_init_target(&ctxt, IMAGE_SIZE, IMAGE_SIZE, nodes, NODE_COUNT);

    stbrp_pack_rects(&ctxt, imgs_rects, imgs_index);

    //printf("Pack done : \n");
    for (int i = 0; i < imgs_index; i++)
    {
        
        stbrp_rect* rect    = &imgs_rects[i];
        Img* img        = &imgs[rect->id];
        
        if (rect->was_packed)
        {
            //printf("    %s x:%d y:%d\n", img->name, rect->x, rect->y);

            // Copy image onto destination
            cpy_image(img->data, 
                        img->w, 
                        img->q, 
                        out_image_data,
                        rect->x + pad, 
                        rect->y + pad, 
                        IMAGE_SIZE, 
                        IMAGE_SIZE);
        }

    }

    stbi_write_png("../test.png", IMAGE_SIZE, IMAGE_SIZE, 4, out_image_data, IMAGE_SIZE * PIXEL_SIZE);

    FindClose(hndl);
}