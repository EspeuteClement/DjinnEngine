#pragma once
#include "types.h"

// Types

#define DJN_SOUND_INVALID_HANDLE ((u32)-1)
struct djn_audio_sound
{
	u32 handle;
};

// Methods

void djn_audio_init();	// Init audio system

void djn_audio_deinit(); // Deinit audio system

djn_audio_sound djn_audio_load_sound(const char* path);	// Load sound into sound manager

void djn_audio_play_sound(djn_audio_sound& to_play);

void djn_audio_unload_sounds();		// Remove all loaded sounds

