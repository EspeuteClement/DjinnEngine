#pragma once
#include <cstdint>
#include <cstdio>

// Types
typedef FILE* pack_file;

struct pack_quad
{
    uint16_t u1, v1, u2, v2;
};


struct pack_data
{
    // Quad to take on the final texture
    pack_quad q;

    // number of pixels that have been trimmed of the x axis on the left of the image
    uint16_t ox;
    // number of pixels that have been trimmed of the y axis of the top of the image
    uint16_t oy;
    // Width of the original sprite
    uint16_t ow;
    // Height of the original sprite
    uint16_t oh;
};

struct pack_name
{
    char name[32];
    uint16_t id;
};

struct pack_final
{
    // Out : Number of images in the pack
    uint16_t num_images;

    // In-Out : The buffers where to store the informations. Allocate at least num_images
    pack_data* pack_data_buffer;
    pack_name* pack_name_buffer;

    // Internal
    pack_file _file_handle;
};

// Open the given file to read. Call this, then allocate the buffers in the given pack_final
// Returns true if the file has been sucessfully opened
bool pack_open(const char * file, pack_final & info, const char * mode);

// Reac all the data in the opened file
void pack_read(pack_final & info);

// Write all the data in the given pack
void pack_write(const pack_final & info);

// Close the file opened with the given pack, but does not free the allocated buffers
void pack_close(pack_final & info);

// Return the id corresponding to the given filename, in the given pack_name buffer
int32_t pack_find(const char * filename, const pack_name* db, size_t db_size);