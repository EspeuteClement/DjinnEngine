#include "windows.h"
#include "game.h"
#include <cstdio>
#include "include/sdl2/SDL.h"

GAME_LOOP(game_loop)
{
    printf("%5d : Bless this mess !\n", memory->x);
    memory->x ++ ;

        SDL_Event event;
    while (SDL_PollEvent(&event)) {
    };
    SDL_Surface* screenSurface = SDL_GetWindowSurface(memory->window);
    uint8_t color = memory->x%256;
    SDL_FillRect(screenSurface, NULL, SDL_MapRGB(screenSurface->format, color, color, color));
    SDL_UpdateWindowSurface(memory->window);
}