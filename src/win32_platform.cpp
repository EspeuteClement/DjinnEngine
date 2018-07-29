#include "windows.h"

#include "external/include/GLFW/glfw3.h"

#include "utils/utils.h"
#include "utils/utils.cpp"

#include "game.h"

#include <cstdio>
#include <cstring>

struct game_code_data
{
    void* lib_handle;
    game_loop_handle*           game_loop;
    game_init_handle*   game_init;
    game_deinit_handle* game_deinit;

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
    Result.game_init = &game_init;
    Result.game_deinit = &Ggame_deinit;
    Result.lib_handle = (void*)1;
#else
    // Parameters
    const char* funcname = "game_loop";
    const char* dll_temp_name = "game_temp.dll";
    const char* dll_name = "game.dll";

    Result.lastWriteTime = Win32GetLastWirteTime(dll_name);
	if (!CopyFileA(dll_name, dll_temp_name, false))
	{
		printf("=== Couldn't copy file ! %d ===\n", GetLastError());
	}
    Result.lib_handle = LoadLibraryA(dll_temp_name);
    
    if (Result.lib_handle)
    {
		printf("=== Game DLL Loaded ! ===\n");
        Result.game_loop            = (game_loop_handle*)GetProcAddress((HMODULE) Result.lib_handle, funcname);
        Result.game_init    = (game_init_handle*)GetProcAddress((HMODULE) Result.lib_handle, "game_init");
        Result.game_deinit  = (game_deinit_handle*)GetProcAddress((HMODULE) Result.lib_handle, "game_init");

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


// indexing system

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


#if 0
void init_input_mapping()
{
    input_mappings = (djnKey*) calloc(1, sizeof(djnKey) * MAX_NUM_PLAYERS * GLFW_KEY_LAST);

    input_mappings[0 * GLFW_KEY_LAST    + GLFW_KEY_UP]      = djnKey::djnKey_UP;
    input_mappings[0 * GLFW_KEY_LAST    + GLFW_KEY_LEFT]    = djnKey::djnKey_LEFT;
    input_mappings[0 * GLFW_KEY_LAST    + GLFW_KEY_DOWN]    = djnKey::djnKey_DOWN;
    input_mappings[0 * GLFW_KEY_LAST    + GLFW_KEY_RIGHT]   = djnKey::djnKey_RIGHT;
    input_mappings[0 * GLFW_KEY_LAST    + GLFW_KEY_X]       = djnKey::djnKey_1;
    input_mappings[0 * GLFW_KEY_LAST    + GLFW_KEY_C]       = djnKey::djnKey_2;
    input_mappings[0 * GLFW_KEY_LAST    + GLFW_KEY_V]       = djnKey::djnKey_3;
    input_mappings[0 * GLFW_KEY_LAST    + GLFW_KEY_SPACE]   = djnKey::djnKey_START;
}

void deinit_input_mapping()
{
    free(input_mappings);
}
#endif

void KeyFunc(GLFWwindow*, int key, int, int action, int mods)
{
    if (memory_ptr->OnKeyCallback)
        memory_ptr->OnKeyCallback(key, action, mods);

    // Simple key input 
/*    for (int player_id = 0; player_id < MAX_NUM_PLAYERS; ++player_id)
    {
        memory_ptr->input._int_set_key(player_id, input_mappings[player_id * GLFW_KEY_LAST + key]);
    }*/
}

void init_memory()
{
    DJN_PERF("init_memory time");

    memory_ptr = (Memory*) malloc(sizeof(Memory));
    memset(memory_ptr, 0, sizeof(Memory));
    memory_ptr->memory_size = sizeof(Memory);

    printf("Game memory footprint is %d kb\n", sizeof(Memory) / 1024);
}

GLFWwindow* window;

int init_glfw()
{
    {
        DJN_PERF("GLFW time");

        /* Initialize the library */
        if (!glfwInit())
            return 0;
        
        /* Create a windowed mode window and its OpenGL context */
        window = glfwCreateWindow(640, 480, "Hello World", NULL, NULL);
        if (!window)
        {
            glfwTerminate();
            return 0;
        }
        char buffer[32];
    }

    glfwWindowHint(GLFW_SAMPLES, 0);
    glfwMakeContextCurrent(window);
    glfwSwapInterval(1);

    return 1;
}

game_code_data code_data;

int init_game()
{
    init_memory();
    code_data = LoadGameCode();

    {
        // Input callbacks
        glfwSetScrollCallback(window, &ScrollFunc);
        glfwSetCharCallback(window, &CharFunc);
        glfwSetKeyCallback(window, &KeyFunc);
    }

    if (!code_data.game_loop)
    {
        return(-1);
    }

    memory_ptr->proc = (void*)glfwGetProcAddress;
    code_data.game_init(memory_ptr);

    return 1;
}



// Recording

#define MAX_RECORDING_INPUTS (1 << 20)

enum RecordStatus
{
    RS_STOPPED,
    RS_PLAYING,
    RS_RECORDING
};

struct GameRecord
{
    Memory memory;
    RecordStatus status;
    s32 currentInputFrame;
    s32 maxInputFrame;
    PlayerInput* inputs[MAX_NUM_PLAYERS];
};

GameRecord* record = nullptr;

void memory_record_start()
{
    if (!record)
    {
        record = (GameRecord*) malloc(sizeof(GameRecord));
        for (int i = 0; i < MAX_NUM_PLAYERS; i++)
        {
            record->inputs[i] = (PlayerInput*) malloc(sizeof(PlayerInput) * MAX_RECORDING_INPUTS);
        }
    }

    record->currentInputFrame = 0;

    for (int i = 0; i < MAX_NUM_PLAYERS; i++)
    {
         memset(record->inputs[i], 0, sizeof(PlayerInput) * MAX_RECORDING_INPUTS);
    }
   

    memcpy((void*)&record->memory, memory_ptr, sizeof(Memory));

    record->status = RS_RECORDING;
}

void memory_record_play()
{
    if (!record)
        return;

    record->status = RS_PLAYING;
    record->maxInputFrame = record->currentInputFrame;
    record->currentInputFrame = 0;

    memcpy(memory_ptr, (void*)&record->memory, sizeof(Memory));
}

void memory_record_step()
{
    if (!record)
        return;

    switch(record->status)
    {
        case RS_RECORDING:
        {

            for (int i = 0; i < MAX_NUM_PLAYERS; i++)
            {
                PlayerInput& pi = record->inputs[i][record->currentInputFrame];
                pi = memory_ptr->input.inputs[i][memory_ptr->input.currentHistoryFrame];
            }
            record->currentInputFrame ++;
        } break;
        case RS_PLAYING:
        {
            for (int i = 0; i < MAX_NUM_PLAYERS; i++)
            {
                PlayerInput& pi = record->inputs[i][record->currentInputFrame];
                memory_ptr->input.inputs[i][memory_ptr->input.currentHistoryFrame] = pi;
            }

            record->currentInputFrame ++;

            if (record->currentInputFrame >= record->maxInputFrame)
            {
                memory_record_play();
            }
        } break;
        default:
        {
        } break;
    }
}

int main(int argc, char* args[])
{
    init_glfw();
    init_game();

/*    init_input_mapping();*/

    /* Loop until the user closes the window */
    while (!glfwWindowShouldClose(window))
    {
        uint64_t start_time = djn::get_time_micro(); 

        // Reset input :
        memory_ptr->input._int_new_frame();

        glfwPollEvents();
        // Update inputs :
        {
            if (glfwGetWindowAttrib(window, GLFW_FOCUSED))
            {
                double x, y;
                glfwGetCursorPos(window, &x, &y);
                memory_ptr->input.mouse_x = x;
                memory_ptr->input.mouse_y = y;

                for (int entry = 0; input_mappings[entry].player != -1; ++entry)
                {
                    InputMapping &m = input_mappings[entry];
                    if (glfwGetKey(window, m.glfw_key))
                    {
                        memory_ptr->input._int_set_key(m.player, m.djn_key);
                    }
                    
                }

                for (int i = 0; i < 5; i++)
                {
                    memory_ptr->input.mouse_btn[i] = glfwGetMouseButton(window, i);
                }

                if (glfwGetKey(window, GLFW_KEY_F10) && (!record || record->status != RS_RECORDING))
                {
                    memory_record_start();
                } 

                if (glfwGetKey(window, GLFW_KEY_F11) && (record && record->status != RS_PLAYING))
                {
                    memory_record_play();
                }
            }
            
        }

        glfwGetFramebufferSize(window, &memory_ptr->screen_width, &memory_ptr->screen_height);

        /* Render here */
        //glClear(GL_COLOR_BUFFER_BIT);
        
        memory_record_step();
        code_data.game_loop(memory_ptr);


#ifndef __STANDALONE__
        FILETIME currentFileTime = Win32GetLastWirteTime("game.dll");
                
        if (CompareFileTime(&currentFileTime, &code_data.lastWriteTime) > 0)
        {
            // Check if we can open the dll
            bool canOpen = false;
            {
                HANDLE fh;
                fh = CreateFile("game.dll", GENERIC_WRITE | GENERIC_READ, 0 , NULL, OPEN_EXISTING, 0, NULL);
                if ((fh != NULL) && (fh != INVALID_HANDLE_VALUE))
                {
                    canOpen = true;
                    CloseHandle(fh);
                }
            }
            
            if (canOpen)
            {
                code_data.game_deinit(memory_ptr);

                UnloadGameCode(code_data);
                code_data = LoadGameCode();
                code_data.lastWriteTime = currentFileTime;

                djnStatus initStatus = code_data.game_init(memory_ptr);
                if (initStatus != djnStatus::djnSta_OK)
                {
                    if (initStatus == djnStatus::djnSta_ERROR_MEMORY)
                    {
                        printf("Error : Memory layout has changed since last reload. Forced to quit game\n");
                        system("PAUSE");
                        return -1;
                    }
                    else
                    {
                        printf("Error : Couldn't re-init game\n");
                        system("PAUSE");
                        return -1;
                    }
                }
            }
        }
#endif

        /* Swap front and back buffers */
        glfwSwapBuffers(window);        

        //memset(&memory_ptr->input, 0, sizeof(memory_ptr->input));
        /* Poll for and process events */

        uint64_t end_time = djn::get_time_micro();
        
        AddDebugFrame(&memory_ptr->debug, (end_time - start_time)/1000.0f);
    }

/*    deinit_input_mapping();*/
    glfwTerminate();
    return 0;
}


///





