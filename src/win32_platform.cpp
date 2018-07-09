#define SDL_MAIN_HANDLED
#include "include/sdl2/SDL.h"
#include <cstdio>

#include "game.h"
#include "windows.h"

#define SCREEN_WIDTH 640
#define SCREEN_HEIGHT 480

struct game_code_data
{
    void* lib_handle;
    game_loop_handle* game_loop;
    FILETIME lastWriteTime = {};
};

FILETIME Win32GetLastWirteTime(const char* filename)
{
    WIN32_FIND_DATAA data = {};
    HANDLE file_handle = FindFirstFileA("game.dll",&data);
    FILETIME currentFileTime = data.ftLastWriteTime;
    FindClose(file_handle);

    return(currentFileTime);
}

game_code_data LoadGameCode()
{


    game_code_data Result = {};



    // Parameters
    const char* funcname = "game_loop";
    const char* dll_temp_name = "game_temp.dll";
    const char* dll_name = "game.dll";

    Result.lastWriteTime = Win32GetLastWirteTime(dll_name);
    CopyFileA(dll_name, dll_temp_name, false);
    Result.lib_handle = SDL_LoadObject(dll_temp_name);
    
    if (Result.lib_handle)
    {
        printf("=== DLL reloaded ===\n");
        Result.game_loop = (game_loop_handle*)SDL_LoadFunction(Result.lib_handle, funcname);
    }

    return(Result);
}

void UnloadGameCode(game_code_data& data)
{
    SDL_UnloadObject(data.lib_handle);
    data.lib_handle = 0;
    data.game_loop = 0;
}

int main(int argc, char* args[])
{
    Memory memory = {};

    memory.window = NULL;

    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        fprintf(stderr, "could not initialize sdl2: %s\n", SDL_GetError());
        return 1;
    }
    memory.window = SDL_CreateWindow(
                    "hello_sdl2",
                    SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
                    SCREEN_WIDTH, SCREEN_HEIGHT,
                    SDL_WINDOW_SHOWN
                    );
    if (memory.window == NULL) {
        fprintf(stderr, "could not create window: %s\n", SDL_GetError());
        return 1;
    }



    game_code_data code_data = LoadGameCode();

    if (code_data.game_loop)
    {
        while(1)
        {
            code_data.game_loop(&memory);
            SDL_Delay(20);
            FILETIME currentFileTime = Win32GetLastWirteTime("game.dll");
            if (CompareFileTime(&currentFileTime, &code_data.lastWriteTime) > 0)
            {
                UnloadGameCode(code_data);
                code_data = LoadGameCode();
                code_data.lastWriteTime = currentFileTime;
            }

        }

    }
    else
    {
        printf("Couldn't load library zerjhzdjkfsdf: %s\n", SDL_GetError());
    }

    SDL_DestroyWindow(memory.window);
        SDL_Quit();
    return(0);
}