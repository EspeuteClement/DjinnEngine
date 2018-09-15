/*
 * Simple Test program for libtcc
 *
 * libtcc can be useful to use tcc as a "backend" for a code generator.
 */
#include <stdlib.h>
#include <stdio.h>

#include "djn_graph.c"

#include <SDL.h>

#include "libtcc.h"

#include "ugl/ugl.c"
#include "djn_debug.c"
#include "djn_imgui.c"

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

typedef struct
{
    // Functions
    void (*main)();
    void (*step)();
    void (*draw)();

    // Data
    void* data_ptr;
    size_t data_size;

    // Compilation state
    TCCState *s;
} game_code;

void load_or_reload_gamecode(game_code* code)
{
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
            free(code->data_ptr);

        fprintf(stderr, "Allocating %d b of data for the game\n", (int)code->data_size);
        
        code->data_ptr = (void*) calloc(code->data_size, 0);

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
    code->main = tcc_get_symbol(code->s, "main");
    code->step = tcc_get_symbol(code->s, "step");
    code->draw = tcc_get_symbol(code->s, "draw");
}

int main(int argc, char **argv)
{
    SDL_GLContext oglContext;

    game_code code = {0};
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

    load_or_reload_gamecode(&code);

    size_t game_data_size = 0;
    void* game_data = NULL;
    void** game_data_symbol = NULL;

    char c = 0;
    int quit = 0;

    djn_imgui_init(window);

    while (quit == 0)
    {
        while(!quit)
        {
            SDL_Event e;
            while (SDL_PollEvent(&e) != 0)
            {
                if (djn_imgui_process_events(&e))
                {
                    continue;
                }

                if (e.type == SDL_QUIT)
                {
                    quit = 1;
                }

                if (e.type == SDL_KEYDOWN)
                {
                    if (e.key.keysym.sym == SDLK_F11)
                    {
                        load_or_reload_gamecode(&code);
                        fprintf(stdout, "Code reloaded\n");
                    }
                }
            }
            djn_imgui_new_frame();
            igNewFrame();


            djn_graph_draw();
            if (code.main) code.main();
            if (code.step) code.step();
            if (code.draw) code.draw();

            static bool open = true;
            igSetNextWindowPos((ImVec2){400,400}, 0, (ImVec2){0,0});
            igBegin("Hello", NULL, ImGuiWindowFlags_AlwaysAutoResize);
                igText("Hello world");
                igText("Hello world");
            igEnd();

            igBegin("Hello 2", NULL, ImGuiWindowFlags_AlwaysAutoResize);
                igText("Hello world");
                igText("Hello world");
                igText("Hello world");
                igText("Hello world");
                igText("Hello world");
            igEnd();
            igShowDemoWindow(NULL);

            igRender();

            //SDL_GL_MakeCurrent(window, gl_context);
            struct ImGuiIO* io = igGetIO();
            //printf("DisplaySize %d %d\n", (int)io->DisplaySize.x, (int)io->DisplaySize.y);
            glViewport(0, 0, (int)io->DisplaySize.x, (int)io->DisplaySize.y);

            ImGui_ImplOpenGL3_RenderDrawData(igGetDrawData());
            SDL_GL_SwapWindow( window );
        }
    }

    //igDestroyContext(struct ImGuiContext *ctx)
    ImGui_ImplOpenGL3_Shutdown();
    /* delete the state */
    tcc_delete(code.s);

    return 0;
}
