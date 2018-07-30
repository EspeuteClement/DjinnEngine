

#include "external/include/GLFW/glfw3.h"

#include "utils/utils.h"
#include "utils/utils.cpp"
#include <emscripten/emscripten.h>
#include <emscripten/html5.h>

#include "game.h"

#include <cstdio>

GameData game_data = {};

GLFWwindow* window;

bool is_focused = true;

EM_JS(int, has_focus, (),
{
    return document.hasFocus();
});

EM_BOOL on_blur(int eventType, const EmscriptenFocusEvent *focusEvent, void *userData)
{
    is_focused = false;
    return true;
}

EM_BOOL on_focus(int eventType, const EmscriptenFocusEvent *focusEvent, void *userData)
{
    is_focused = true;
    return true;
}

struct InputMapping
{
    s8 player;
    u16 glfw_key;
    djnKey djn_key;
};

InputMapping input_mappings[] =
{
    {0  ,GLFW_KEY_UP,       djnKey::djnKey_UP},
    {0  ,GLFW_KEY_LEFT,     djnKey::djnKey_LEFT},
    {0  ,GLFW_KEY_DOWN,     djnKey::djnKey_DOWN},
    {0  ,GLFW_KEY_RIGHT,    djnKey::djnKey_RIGHT},
    {0  ,GLFW_KEY_SPACE,    djnKey::djnKey_START},
    {0  ,GLFW_KEY_X,        djnKey::djnKey_1},
    {0  ,GLFW_KEY_C,        djnKey::djnKey_2},
    {0  ,GLFW_KEY_V,        djnKey::djnKey_3},
    {-1  ,0,               djnKey::djnKey_NONE}
};

void renderFrame()
{
    glfwPollEvents();
    game_data.djn_memory.input._int_new_frame();

    for (int entry = 0; input_mappings[entry].player != -1; ++entry)
    {
        InputMapping &m = input_mappings[entry];
        if (glfwGetKey(window, m.glfw_key))
        {
            game_data.djn_memory.input._int_set_key(m.player, m.djn_key);
        }
    }

    double x, y;
    glfwGetCursorPos(window, &x, &y);
    game_data.djn_memory.input.mouse_x = x;
    game_data.djn_memory.input.mouse_y = y;

    for (int i = 0; i < 5; i++)
    {
        game_data.djn_memory.input.mouse_btn[i] = glfwGetMouseButton(window, i);
    }

    game_data.is_system_paused = !has_focus();

    glfwGetFramebufferSize(window, &game_data.screen_width, &game_data.screen_height);
    game_loop(&game_data);
    glfwSwapBuffers(window);
  


}

int main(void)
{
    //emscripten_set_blur_callback(0, 0, 1, on_blur);
    //emscripten_set_focus_callback(0, 0, 1, on_focus);

    /* Initialize the library */
    if (!glfwInit()) {
        printf("Could not initialize library\n");
        return -1;
    }

    /* Create a windowed mode window and its OpenGL context */
    window = glfwCreateWindow(640, 480, "Hello World", NULL, NULL);
    if(!window)
    {
        printf("Could not create OpenGL window\n");
        glfwTerminate();
        return -1;
    }

    glfwWindowHint(GLFW_SAMPLES, 0);
    glfwMakeContextCurrent(window);


    game_data.proc = (void*)glfwGetProcAddress;

    game_data.memory_size = sizeof(GameData);
    game_init(&game_data);


    emscripten_set_main_loop(&renderFrame,0,1);

    glfwTerminate();
    return 0;
}