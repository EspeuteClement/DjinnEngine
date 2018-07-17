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

Memory* memory_ptr;

void ScrollFunc(GLFWwindow* window,double xoffset,double yoffset)
{
    memory_ptr->input.mouse_sx = (float)xoffset;
    memory_ptr->input.mouse_sy = (float)yoffset; 
}

void CharFunc(GLFWwindow* window, unsigned int c)
{
    if (memory_ptr->OnCharInputCallback)
        memory_ptr->OnCharInputCallback(c);
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

        glfwWindowHint(GLFW_SAMPLES, 0);

    /* Make the window's context current */
    glfwMakeContextCurrent(window);

    Memory memory = {};
    memory_ptr = &memory;

    game_code_data code_data = LoadGameCode();

    {
        // Input callbacks
        glfwSetScrollCallback(window, &ScrollFunc);
        glfwSetCharCallback(window, &CharFunc);
    }
    

    if (!code_data.game_loop)
    {
        return(-1);
    }

    memory.proc = (void*)glfwGetProcAddress;
    code_data.game_init_graphic(&memory);

    
    int frame_acc = 0;
    glfwSwapInterval(1);
    /* Loop until the user closes the window */
    while (!glfwWindowShouldClose(window))
    {
        uint64_t start_time = djn::get_time_micro(); 
        frame_acc ++;

        // Update inputs :
        {
            if (glfwGetWindowAttrib(window, GLFW_FOCUSED))
            {
                double x, y;
                glfwGetCursorPos(window, &x, &y);
                memory.input.mouse_x = x;
                memory.input.mouse_y = y;

                for (int i = 0; i < 5; i++)
                {
                    memory.input.mouse_btn[i] = glfwGetMouseButton(window, i);
                } 
            }
            
        }

        glfwGetFramebufferSize(window, &memory.screen_width, &memory.screen_height);

        /* Render here */
        //glClear(GL_COLOR_BUFFER_BIT);
       
        code_data.game_loop(&memory);


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



        AddDebugFrame(&memory.debug, (end_time - start_time)/1000.0f);
        

        memset(&memory.input, 0, sizeof(memory.input));
        /* Poll for and process events */
        glfwPollEvents();

        uint64_t end_time = djn::get_time_micro();

        if (frame_acc >= 0)
        {
            //if ((end_time - start_time) > 20000)
                printf("Frame_time = %d\n" , (uint32_t) (end_time - start_time));
            frame_acc = 0;
        }
    }

    glfwTerminate();
    return 0;
}


///





