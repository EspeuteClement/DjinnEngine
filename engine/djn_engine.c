/*
 * Simple Test program for libtcc
 *
 * libtcc can be useful to use tcc as a "backend" for a code generator.
 */
#include <stdlib.h>
#include <stdio.h>

#include <SDL.h>

#ifndef DJN_NO_RELOAD
#include "libtcc.h"
#else
#include "djn_game.h"
#endif

#include "cimgui/djn_imgui.h"
#include "djn_resources.h"
#include "djn_alloc.h"
#include "djn_graph.h"
#include "djn_sprite_batcher.h"

#include "../game/game_resources.h"

#define TRACE fprintf(stderr, "Trace : %s(%d)\n", __FILE__, __LINE__);

#define sizeof_array(x) (sizeof(x)/sizeof(x[0]))

#ifndef DJN_NO_RELOAD
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
        "game/game_main.c",
        "game/game_resources.c",
    };

    const char * _lib_paths[] =
    {
        "engine/sdl/lib/x64", // We don't need to link to sdl at the moment
        "engine/tcc/lib",
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


    #define _TYPEDECL(x)
    #define _FUNCNAME(name) {&name, #name}
    #define _PARAMS(x)
    #define _END ,

    typedef struct
    {
        void *ptr;
        const char* name;
    } symbol;

    /*const symbol _ugl_symbols[] =
    {
        #include "ugl/ugl.incl"
    };*/

    const symbol _symbols[] =
    {
        {draw_triangle, "draw_triangle"},
        {draw_quad, "draw_quad"},
        {resource_load_spritesheet, "resource_load_spritesheet"},
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

        tcc_add_sysinclude_path(s, "engine/tcc/include");
        tcc_add_include_path(s, "engine");


        // Add all ugl symbols
        for (int i = 0; i < sizeof_array(_symbols); i++)
        {
            const symbol * _symbol = &_symbols[i];
            if (tcc_add_symbol(s, _symbol->name, _symbol->ptr) == -1)
            {
                fprintf(stderr, "Could not add symbol %s\n", _symbol->name);
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
#endif

typedef struct
{
    // Functions
    void (*init)();
    void (*step)();
    void (*draw)();

    // Data
    void* data_ptr;
    size_t data_size;

    // Compilation state
#ifndef DJN_NO_RELOAD
    TCCState *s;
#endif
} game_code;

void load_or_reload_gamecode(game_code* code)
{
    // TCC loading
#ifndef DJN_NO_RELOAD

    TCCState *new_state = load_game_code();
    if (new_state)
    {
        if (code->s)
        {
            tcc_delete(code->s);
        }

        code->s = new_state;
    }
    else
    {
        if (code->s)
        {
            fprintf(stdout, "Couldn't compile a new version. Will use old version of code ...\n");
        }
        else
        {
            fprintf(stdout, "Couldn't compile ...\n");
            return;
        }
    }

    size_t* ptr = (size_t*) tcc_get_symbol(code->s, "game_data_size");
    if (!ptr)
    {
        fprintf(stderr, "Could not find symbol game_data_size\n");
        return;
    }

    size_t new_game_data_size = (*ptr);

    if (!code->data_ptr || new_game_data_size!=code->data_size)
    {

        code->data_size = new_game_data_size;

        if (code->data_ptr)
            djn_free(code->data_ptr);

        fprintf(stderr, "Allocating %d b of data for the game\n", (int)code->data_size);
        
        code->data_ptr = (void*) djn_calloc(code->data_size);

        if (!code->data_ptr)
        {
            fprintf(stderr, "Could not re-allocate memory for game\n");
            return;
        }
    }

    void ** data_symbol = tcc_get_symbol(code->s, "game_data");
    if (!data_symbol)
    {
        fprintf(stderr, "Could not find game_data symbol\n");
        return;
    }
    
    *data_symbol = code->data_ptr;

    /* get entry symbol */

    code->init = tcc_get_symbol(code->s, "init");
    code->step = tcc_get_symbol(code->s, "step");
    code->draw = tcc_get_symbol(code->s, "draw");


    const int* enum_test = tcc_get_symbol(code->s, "game_spritesheets_count");
    if (enum_test)
    {
        game_current_texture_count = *enum_test;
    }
    else
    {
        printf("warning : game_spritesheets_count is not defined\n");
        game_current_texture_count = 0;
    }

    game_current_spritesheet_data = tcc_get_symbol(code->s, "spritesheets_paths");
    if (!game_current_spritesheet_data)
    {
        printf("warning : spritesheets_paths is not defined\n");
        game_current_spritesheet_data = NULL;
    }

    // Free old sprite resources
    resource_free_spritesheets();
#else   // Non TCC loading
        code->data_size = game_data_size;

        if (code->data_ptr)
            djn_free(code->data_ptr);

        fprintf(stderr, "Allocating %d b of data for the game\n", (int)code->data_size);
        
        code->data_ptr = (void*) djn_calloc(code->data_size);

        if (!code->data_ptr)
        {
            fprintf(stderr, "Could not re-allocate memory for game\n");
            return;
        }

		game_data = code->data_ptr;

		code->init = &init;
		code->step = &step;
		code->draw = &draw;

		game_current_texture_count = game_spritesheets_count;
		game_current_spritesheet_data = spritesheets_paths;
#endif
}

typedef struct
{
    int quit;
    game_code code;
} game_state;

void djn_engine_inputs(game_state* state)
{
    SDL_Event e;
    while (SDL_PollEvent(&e) != 0)
    {
#ifndef DJN_NO_IMGUI
        djn_imgui_process_events(&e);
#endif
        if (e.type == SDL_QUIT)
        {
            state->quit = 1;
        }

        if (e.type == SDL_KEYDOWN)
        {
#ifndef DJN_NO_RELOAD
            if (e.key.keysym.sym == SDLK_F11)
            {
                load_or_reload_gamecode(&state->code);
                fprintf(stdout, "Code reloaded\n");
            }
#endif
        }
    }
}

void djn_engine_deinit()
{
#ifndef DJN_NO_IMGUI
    djn_imgui_deinit();
#endif
    resource_free_spritesheets();
}

void djn_engine_frame_begin()
{
#ifdef WITH_IMGUI
    djn_imgui_new_frame();
    igNewFrame();
#endif
}

int main(int argc, char **argv)
{
	printf("%s\n", argv[0]);
    djn_sprite_batch_dir("res/to_pack", 0);

    SDL_GLContext oglContext;

    game_state state = {0};
    SDL_Window* window = NULL;
    SDL_Surface* screenSurface = NULL;

    if (SDL_Init(SDL_INIT_VIDEO) < 0)
    {
        fprintf(stderr, "SDL could not init. Error : %s\n", SDL_GetError());
        exit(-1);
    }

    

    SDL_GL_SetAttribute( SDL_GL_CONTEXT_MAJOR_VERSION, 3 ); 
    SDL_GL_SetAttribute( SDL_GL_CONTEXT_MINOR_VERSION, 2 );

    window = SDL_CreateWindow(
                "Djinn Engine", 
                SDL_WINDOWPOS_UNDEFINED, 
                SDL_WINDOWPOS_UNDEFINED, 
                SCREEN_W, SCREEN_H, SDL_WINDOW_OPENGL);

    if (window == NULL)
    {
        fprintf(stderr, "SDL could not create window. Error : %s\n", SDL_GetError());
        exit(-1);
    }

    oglContext = SDL_GL_CreateContext(window);

    if (oglContext == NULL)
    {
        fprintf(stderr, "OpenGL context could not be created. Error : %s\n", SDL_GetError());
        exit(-1);
    }

    if( SDL_GL_SetSwapInterval( 1 ) < 0 ) 
    { 
        fprintf(stderr, "Warning: Unable to set VSync! SDL Error: %s\n", SDL_GetError() ); 
    }

    uGlLoadGL(&SDL_GL_GetProcAddress);

    djn_graph_init();
    

    load_or_reload_gamecode(&state.code);

    char c = 0;
    int quit = 0;
#ifndef DJN_NO_IMGUI
    djn_imgui_init(window);
#endif
    if (state.code.init) state.code.init();

    while(!state.quit)
    {
        djn_engine_inputs(&state);
        djn_engine_frame_begin();
    

        djn_graph_draw();
        if (state.code.step) state.code.step();
        if (state.code.draw) state.code.draw();
    

#ifdef WITH_IMGUI
		static bool open = true;
        if(igBegin("Hello", NULL, ImGuiWindowFlags_AlwaysAutoResize))
        {
            int value = igButton("Free Textures", (ImVec2){0.0f, 0.0f});
            if(value == true)
            {
                resource_free_spritesheets();
            }
            else if(value != false)
            {
                printf("Button wtf %d\n", value);
            }
            resource_load_spritesheet(0);
            igImage((void*)(storage[0].gl_texture), (ImVec2){1024,1024},(ImVec2){0.0f,0.0f} ,(ImVec2){1.0f,1.0f},(ImVec4){1.0f,1.0f,1.0f,1.0f},(ImVec4){1.0f,1.0f,1.0f,1.0f});
        }
        igEnd();

    


        igShowDemoWindow(NULL);
#endif
    

#ifndef DJN_NO_IMGUI
		igRender();

        //SDL_GL_MakeCurrent(window, gl_context);
        struct ImGuiIO* io = igGetIO();
        //printf("DisplaySize %d %d\n", (int)io->DisplaySize.x, (int)io->DisplaySize.y);
        glViewport(0, 0, (int)io->DisplaySize.x, (int)io->DisplaySize.y);

        djn_imgui_draw_data(igGetDrawData());
    

#endif
        
        SDL_GL_SwapWindow( window );
    }
    
    
    if (state.code.data_ptr)
    {
        djn_free(state.code.data_ptr);
    }
    

#ifndef DJN_NO_RELOAD
    if (state.code.s)
    {
        tcc_delete(state.code.s);
    }
#endif
    

    djn_engine_deinit();

    djn_memory_leak_report();

    return 0;
}
