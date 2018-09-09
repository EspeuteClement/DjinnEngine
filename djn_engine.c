/*
 * Simple Test program for libtcc
 *
 * libtcc can be useful to use tcc as a "backend" for a code generator.
 */
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "libtcc.h"

#define sizeof_array(x) (sizeof(x)/sizeof(x[0]))

TCCState* load_game_code()
{
    TCCState *s = NULL;

    // ----------------------------------
    // Config
    // ----------------------------------
    const char * _system_includepaths[] =
    {
        "sdl/include",
        "tcc/include",
    };

    const char * _sources[] =
    {
        "game_main.c",
    };

    const char * _lib_paths[] =
    {
        "sdl/lib/x64", // We don't need to link to sdl at the moment
        "tcc/lib",
    };

    const char * _lib_link[] =
    {
        "SDL2",
    };

    const char * _defines[][2] =
    {
        {"_STDINT_H_", ""},
        {"SDL_MAIN_HANDLED", ""},
    };

    s = tcc_new();
    // Configure and compile the game
    {
        if (!s) {
            fprintf(stderr, "Could not create tcc state\n");
            exit(1);
        }
        /* MUST BE CALLED before any compilation */
        tcc_set_output_type(s, TCC_OUTPUT_MEMORY);

        for (int i = 0; i < sizeof_array(_defines); i++)
        {
            const char ** _def = _defines[i];
            tcc_define_symbol(s, _def[0], _def[1]);
        }
        //tcc_set_options(s, "-g");

        for (int i = 0; i < sizeof_array(_system_includepaths); i++)
        {
            const char * _path = _system_includepaths[i];
            if (tcc_add_sysinclude_path(s, _path) == -1)
            {
                fprintf(stderr, "Could not open library %s\n", _path);
                tcc_delete(s);
                return NULL;
            }
        }

        for (int i = 0; i < sizeof_array(_sources); i++)
        {
            const char * _path = _sources[i];
            if (tcc_add_file(s, _path) == -1)
            {
                fprintf(stderr, "Could not compile file %s\n", _path);
                tcc_delete(s);
                return NULL;
            }
        }

        for (int i = 0; i < sizeof_array(_lib_paths); i++)
        {
            const char * _path = _lib_paths[i];
            if (tcc_add_library_path(s, _path) == -1)
            {
                fprintf(stderr, "Could not add libpath %s\n", _path);
                tcc_delete(s);
                return NULL;
            }
        }
        
        for (int i = 0; i < sizeof_array(_lib_link); i++)
        {
            const char * _path = _lib_link[i];
            if (tcc_add_library(s, _path) == -1)
            {
                fprintf(stderr, "Could not add library %s\n", _path);
                tcc_delete(s);
                return NULL;
            }
        }

        /* relocate the code */
        if (tcc_relocate(s, TCC_RELOCATE_AUTO) < 0)
        {
            fprintf(stderr, "Could not realocate\n");
            tcc_delete(s);
            return NULL;
        }
    }

    return s;
}


int main(int argc, char **argv)
{
    TCCState *s = NULL;
    int (*game_main)();

    size_t game_data_size = 0;
    void* game_data = NULL;
    void** game_data_symbol = NULL;

    char c = 0;

    while (c != 'q')
    {
        TCCState *new_state = load_game_code();
        if (new_state)
        {
            if (s)
            {
                tcc_delete(s);
            }

            s = new_state;
        }
        else
        {
            if (s)
            {
                fprintf(stdout, "Couldn't compile a new version. Will use old version of code ...\n");
            }
            else
            {
                fprintf(stdout, "Couldn't compile. Press enter to continue ...\n");

                c = getc(stdin);
                continue;
            }

        }

        /* get entry symbol */
        game_main = tcc_get_symbol(s, "main");
        if (!game_main)
        {
            fprintf(stderr, "Could not find main\n");
            return -5;
        }

        size_t* ptr = (size_t*) tcc_get_symbol(s, "game_data_size");
        if (!ptr)
        {
            fprintf(stderr, "Could not find symbol game_data_size\n");
            return -6;
        }

        size_t new_game_data_size = (*ptr);
        

        if (!game_data || new_game_data_size!=game_data_size)
        {

            game_data_size = new_game_data_size;

            if (game_data)
                free(game_data);

            fprintf(stderr, "Allocating %d b of data for the game\n", (int)game_data_size);
            
            game_data = (void*) malloc(game_data_size);

            if (!game_data)
            {
                fprintf(stderr, "Could not re-allocate memory for game\n");
                return -6;
            }
        }

        game_data_symbol = tcc_get_symbol(s, "game_data");

        *game_data_symbol = (void*) game_data;

        /* run the code */
        game_main();
        fprintf(stdout, "Waiting for input ...\n");
        c = getc(stdin);
    }

    /* delete the state */
    tcc_delete(s);

    return 0;
}
