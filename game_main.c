// SDL2 Hello, World!
// This should display a white screen for 2 seconds
// compile with: clang++ main.cpp -o hello_sdl2 -lSDL2
// run with: ./hello_sdl2
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#define SDL_DISABLE_IMMINTRIN_H
#include <SDL.h>

#define SCREEN_WIDTH 640
#define SCREEN_HEIGHT 480

typedef struct
{
  int data1;
  int data2;
  uint16_t data3;
} str;

size_t game_data_size = sizeof(str);
str* game_data = NULL;

void step() {
  game_data->data1 ++;
  fprintf(stdout, "This is a new code : %d\n", game_data->data1);
}



  /*SDL_Window* window = NULL;
  SDL_Surface* screenSurface = NULL;
  if (SDL_Init(SDL_INIT_VIDEO) < 0) {
    fprintf(stderr, "could not initialize sdl2: %s\n", SDL_GetError());
    return 1;
  }
  window = SDL_CreateWindow(
			    "hello_sdl2",
			    SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
			    SCREEN_WIDTH, SCREEN_HEIGHT,
			    SDL_WINDOW_SHOWN
			    );
  if (window == NULL) {
    fprintf(stderr, "could not create window: %s\n", SDL_GetError());
    return 1;
  }
  screenSurface = SDL_GetWindowSurface(window);
  SDL_FillRect(screenSurface, NULL, SDL_MapRGB(screenSurface->format, 0xFF, 0xFF, 0xFF));
  SDL_UpdateWindowSurface(window);
  SDL_Delay(2000);
  SDL_DestroyWindow(window);
  SDL_Quit();
  return 0;*/