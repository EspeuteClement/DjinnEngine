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
    uint16_t x;
    uint16_t y;
    uint16_t w;
    uint16_t h;
    pack_quad q;
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
void pack_close(const pack_final & info);

// Return the id corresponding to the given filename, in the given pack_name buffer
int32_t pack_find(const char * filename, pack_name db, size_t db_size);