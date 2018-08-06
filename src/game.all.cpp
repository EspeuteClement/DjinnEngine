// Config

#define STB_VORBIS_NO_INTEGER_CONVERSION
#define STB_VORBIS_NO_PUSHDATA_API
#define STB_VORBIS_HEADER_ONLY

#define MAL_NO_WASAPI
#define MAL_NO_WINMM
#define MAL_NO_ALSA
#define MAL_NO_PULSEAUDIO
#define MAL_NO_JACK
#define MAL_NO_COREAUDIO
#define MAL_NO_OSS
#define MAL_NO_OPENSL
#define MAL_NO_NULL
#define MAL_NO_DECODING
#define MAL_NO_STDIO

#define MAL_BASE_BUFFER_SIZE_IN_MILLISECONDS_LOW_LATENCY 4
#define MAL_BASE_BUFFER_SIZE_IN_MILLISECONDS_CONSERVATIVE 4
#define MINI_AL_IMPLEMENTATION
#include "external/mini_al/mini_al.h"
#undef MINI_AL_IMPLEMENTATION


#define DR_WAV_IMPLEMENTATION
#include "external/mini_al/dr_wav.h"
#undef DR_WAV_IMPLEMENTATION


#include "game.cpp"
#include "graph.cpp"
#include "audio.cpp"

#include "external/ugl.cpp"
#include "tools/djinn_pack_api.cpp"
#include "external/stb_vorbis.c"

#include "external/imgui/imgui.cpp"
#include "external/imgui/imgui_draw.cpp"
#include "external/imgui/imgui_impl_djinn.cpp"

#undef STB_VORBIS_HEADER_ONLY
#include "external/stb_vorbis.c"
#define STB_VORBIS_HEADER_ONLY
#undef L
#undef R
#undef C