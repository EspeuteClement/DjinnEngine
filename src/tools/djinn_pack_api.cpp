#include "djinn_pack_api.h"

#include <cstdio>
#include <cassert>
#include <cstring>

bool pack_open(const char * file, pack_final & info, const char * mode)
{
    pack_file fh;

    fh = fopen(file, mode);
    info._file_handle = fh;

    /*if (feof(fh)!=0)
    {
        return false;
    }*/

    fscanf_s(info._file_handle, "%04hX\n", &info.num_images);

    return true;
}

void pack_read(pack_final & info)
{
    assert(info.pack_data_buffer != nullptr && info.pack_name_buffer != nullptr);
    pack_file fh = info._file_handle;

    for (int i = 0; i < info.num_images; i++)
    {
        pack_data& data = info.pack_data_buffer[i];
        fscanf_s(fh,"%04hX%04hX%04hX%04hX%04hX%04hX%04hX%04hX\n", &data.q.u1, &data.q.v1, &data.q.u2, &data.q.v2,&data.ox ,&data.oy, &data.ow, &data.oh );
    }

    for (int i = 0; i < info.num_images; i++)
    {
        pack_name& data = info.pack_name_buffer[i];
        fscanf_s(fh, "%04hX %s\n", &data.id, &data.name, 32);
    }
}

void pack_write(const pack_final & info)
{
    assert(info.num_images != 0);
    assert(info.pack_data_buffer != nullptr && info.pack_name_buffer != nullptr);

    FILE* fh = info._file_handle;

    fprintf(fh, "%04hX\n", info.num_images);

    for (int i = 0; i < info.num_images; i++)
    {
        pack_data& data = info.pack_data_buffer[i];
        fprintf(fh,"%04hX%04hX%04hX%04hX%04hX%04hX%04hX%04hX\n", data.q.u1, data.q.v1, data.q.u2, data.q.v2,data.ox ,data.oy, data.ow, data.oh );
    }

    for (int i = 0; i < info.num_images; i++)
    {
        pack_name& data = info.pack_name_buffer[i];
        fprintf_s(fh, "%04hX %s\n", data.id, data.name);
    }
}

void pack_close(pack_final & info)
{
    fclose(info._file_handle);
    info._file_handle = nullptr;
    return;
}

int32_t pack_find(const char * filename, const pack_name* db, size_t db_size)
{
    // The db should be sorted alphabeticaly, so we can do a binary serach to go faster
    
    int32_t start = 0;
    int32_t end = db_size - 1;

    while (start <= end)
    {
        int32_t pos = (start + end) / 2;

        pack_name data = db[pos];
        int dir = strcmp(data.name, filename);
        
        if (dir < 0)
        {
            start = pos + 1;
        }
        else if (dir > 0)
        {
            end = pos - 1;
        }
        else
        {
            return data.id;
        }
    }

    return -1;
}