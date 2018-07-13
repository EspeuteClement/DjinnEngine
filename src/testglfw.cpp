#include "windows.h"

#include "external/include/GLFW/glfw3.h"

#include "utils/utils.h"
#include "utils/utils.cpp"

#include "game.h"

#include <cstdio>

struct game_code_data
{
    void* lib_handle;
    game_loop_handle*           game_loop;
    game_init_graphic_handle*   game_init_graphic;
    game_unload_graphic_handle* game_unload_graphic;

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

#ifdef __STANDALONE__
    Result.game_loop = &game_loop;
    Result.game_init_graphic = &game_init_graphic;
    Result.game_unload_graphic = &game_unload_graphic;
    Result.lib_handle = (void*)1;
#else
    // Parameters
    const char* funcname = "game_loop";
    const char* dll_temp_name = "game_temp.dll";
    const char* dll_name = "game.dll";

    Result.lastWriteTime = Win32GetLastWirteTime(dll_name);
	if (!CopyFileA(dll_name, dll_temp_name, false))
	{
		printf("=== Couldn't copy file ! ===\n");
	}
    Result.lib_handle = LoadLibraryA(dll_temp_name);
    
    if (Result.lib_handle)
    {
		printf("=== Game DLL Loaded ! ===\n");
        Result.game_loop            = (game_loop_handle*)GetProcAddress((HMODULE) Result.lib_handle, funcname);
        Result.game_init_graphic    = (game_init_graphic_handle*)GetProcAddress((HMODULE) Result.lib_handle, "game_init_graphic");
        Result.game_unload_graphic  = (game_unload_graphic_handle*)GetProcAddress((HMODULE) Result.lib_handle, "game_unload_graphic");

    }
#endif
    return(Result);
}

void UnloadGameCode(game_code_data& data)
{
#ifndef __STANDALONE__
    FreeLibrary((HMODULE) data.lib_handle);
    data.lib_handle = 0;
    data.game_loop = 0;
#endif
}


int main(int argc, char* args[])
{
    GLFWwindow* window;

    /* Initialize the library */
    if (!glfwInit())
        return -1;

    /* Create a windowed mode window and its OpenGL context */
    window = glfwCreateWindow(640, 480, "Hello World", NULL, NULL);
    if (!window)
    {
        glfwTerminate();
        return -1;
    }

    /* Make the window's context current */
    glfwMakeContextCurrent(window);

    Memory memory = {};

    game_code_data code_data = LoadGameCode();

    if (!code_data.game_loop)
    {
        return(-1);
    }

    memory.proc = (void*)glfwGetProcAddress;
    code_data.game_init_graphic(&memory);

    /* Loop until the user closes the window */
    while (!glfwWindowShouldClose(window))
    {
        glfwGetFramebufferSize(window, &memory.screen_width, &memory.screen_height);

        /* Render here */
        //glClear(GL_COLOR_BUFFER_BIT);

        code_data.game_loop(&memory);

        djn::sleep(16);

#ifndef __STANDALONE__
        FILETIME currentFileTime = Win32GetLastWirteTime("game.dll");
                
        if (CompareFileTime(&currentFileTime, &code_data.lastWriteTime) > 0)
        {
            code_data.game_unload_graphic(&memory);

            UnloadGameCode(code_data);
            code_data = LoadGameCode();
            code_data.lastWriteTime = currentFileTime;

            code_data.game_init_graphic(&memory);
        }
#endif


        /* Swap front and back buffers */
        glfwSwapBuffers(window);

        /* Poll for and process events */
        glfwPollEvents();
    }

    glfwTerminate();
    return 0;
}


///





