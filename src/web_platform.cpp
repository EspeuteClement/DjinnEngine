

#include <GL/glfw.h>

#include "utils/utils.h"
#include "utils/utils.cpp"
#include <emscripten/emscripten.h>

#include "game.h"

#include <cstdio>

Memory memory = {};

void renderFrame()
{
    glfwGetWindowSize(&memory.screen_width, &memory.screen_height);
    game_loop(&memory);
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


    memory.proc = (void*)glfwGetProcAddress;
    game_init_graphic(&memory);


    emscripten_set_main_loop(&renderFrame,0,1);

    glfwTerminate();
    return 0;
}