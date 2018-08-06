#pragma once
#include "tools/djinn_pack_api.h"
#include "graph.h"
#include "types.h"

#define MAX_NUM_PLAYERS 1       // Max players input stored

#define MAX_INPUT_HISTORY 60    // Max input stored by the engine for the 
                                // normal Game

struct PlayerInput
{
    u32 Keyflags;    // Keys pressed by the player as flags.

    //@TODO : Axis
};

enum djnKey
{
    djnKey_NONE     = 0,
    djnKey_UP       = 1 << 0,
    djnKey_LEFT     = 1 << 1,
    djnKey_DOWN     = 1 << 2,
    djnKey_RIGHT    = 1 << 3,
    djnKey_1        = 1 << 4,
    djnKey_2        = 1 << 5,
    djnKey_3        = 1 << 6,
    djnKey_4        = 1 << 7,

    djnKey_SELECT   = 1 << 30,
    djnKey_START    = 1 << 31,
};

enum djnStatus
{
    djnSta_ERROR_MEMORY     = -1,
    djnSta_ERROR_UNKNOWN    = -2,
    djnSta_OK               = 1
};

struct Input
{
    r32 mouse_x;
    r32 mouse_y;
    int mouse_btn[5];

    float mouse_sx;
    float mouse_sy;

    u16 currentHistoryFrame;    // Position of the frame in the input history
    PlayerInput inputs[MAX_NUM_PLAYERS][MAX_INPUT_HISTORY];

    inline bool key(u8 player, u8 key) 
    {
        return inputs[player][currentHistoryFrame].Keyflags & key; 
    };

    inline void _int_set_key(u8 player, u8 key)
    {
        inputs[player][currentHistoryFrame].Keyflags |= key;
    };

    inline void _int_new_frame()
    {
        currentHistoryFrame = (currentHistoryFrame + 1) % MAX_INPUT_HISTORY;
        for (int player_id = 0; player_id < MAX_NUM_PLAYERS; ++player_id)
        {
            inputs[player_id][currentHistoryFrame].Keyflags = 0;
        }

        mouse_x = 0;
        mouse_y = 0;
        for (int i = 0; i < sizeof(mouse_btn) / sizeof(mouse_btn[0]); i++)
        {
            mouse_btn[i] = 0; 
        }

        float mouse_sx = 0.0f;
        float mouse_sy = 0.0f;
    }
};


enum GameDrawMode
{
    GDM_Normal,
    GDM_Window,
    GDM_Count
};


#define DEBUG_FRAMES_COUNT 60
struct Debug
{
    r32 lastFrameTimesMs[DEBUG_FRAMES_COUNT];
    int currentTimeFrame;
    GameDrawMode game_draw_mode;
};

inline void AddDebugFrame(Debug *debug, r32 lastTime)
{
    debug->lastFrameTimesMs[debug->currentTimeFrame] = lastTime;
    debug->currentTimeFrame = (debug->currentTimeFrame + 1) % DEBUG_FRAMES_COUNT;
}


struct GameMemory
{
    s32 x;
    s32 y;

    Input input;
};

// GameData ====================================================================
struct GameData
{
    u32 memory_size;    // Used to detect if game_data size has changed
                        // Should always be the first thing in game_data

    GameMemory djn_memory;

    bool is_system_paused; // if true, nothing should update
    
    s32 screen_width;
    s32 screen_height;



    // These function are ment to be called from the main application when the envents are encountered
    void (*OnCharInputCallback) (u32 inputChar);
    void (*OnKeyCallback) (int key, int action, int mods);

    void* proc;

    Debug debug;
};



// Note : Never store this pointer !!! It can become invalid at any moment
extern GameData* djn_game_data;

inline GameMemory* djn_memory()
{
    return &djn_game_data->djn_memory;
}

#ifdef __STANDALONE__
    #define GAME_API extern
#else
    #define GAME_API extern "C" __declspec(dllexport)
#endif

#define GAME_LOOP(name) void name(GameData* game_data)
typedef GAME_LOOP(game_loop_handle);
GAME_API GAME_LOOP(game_loop);

#define GAME_INIT(name) djnStatus name(GameData* game_data)
typedef GAME_INIT(game_init_handle);
GAME_API GAME_INIT(game_init);

#define GAME_DEINIT(name) void name(GameData* game_data)
typedef GAME_DEINIT(game_deinit_handle);
GAME_API GAME_DEINIT(game_deinit);