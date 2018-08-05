// Config

#define STB_VORBIS_NO_INTEGER_CONVERSION
#define STB_VORBIS_NO_PUSHDATA_API

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

#include "game.cpp"
#include "graph.cpp"

#include "external/ugl.cpp"
#include "tools/djinn_pack_api.cpp"
#include "external/stb_vorbis.c"

#include "external/imgui/imgui.cpp"
#include "external/imgui/imgui_draw.cpp"
#include "external/imgui/imgui_impl_djinn.cpp"

#undef STB_VORBIS_HEADER_ONLY
#include "external/stb_vorbis.c"
#undef L
#undef R
#undef C