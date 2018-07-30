

#include "external/glfw/glfw.h"

#include "utils/utils.h"
#include "utils/utils.cpp"
#include <emscripten/emscripten.h>

#include "game.h"

#include <cstdio>

GameData game_data = {};

void renderFrame()
{
    glfwGetWindowSize(&game_data.screen_width, &game_data.screen_height);
    game_loop(&game_data);
    glfwSwapBuffers();
}

int main(void)
{
    /* Initialize the library */
    if (!glfwInit()) {
        printf("Could not initialize library\n");
        return -1;
    }

    /* Create a windowed mode window and its OpenGL context */
    if(glfwOpenWindow(640, 480, 8,8,8,8,0,0, GLFW_WINDOW) != GL_TRUE)
    {
        printf("Could not create OpenGL window\n");
        glfwTerminate();
        return -1;
    }


    game_data.proc = (void*)glfwGetProcAddress;

    game_data.memory_size = sizeof(GameData);
    game_init(&game_data);


    emscripten_set_main_loop(&renderFrame,0,1);

    glfwTerminate();
    return 0;
}