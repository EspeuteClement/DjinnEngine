#include "game.h"
#include "audio.h"

#include "external/mini_al/mini_al.h"
#include "external/stb_vorbis.c"
#include "external/mini_al/dr_wav.h"


#define SAMPLE_RATE 44100

float volume = 1.0f;

// Private functions
void scale_buffer(float* in_buffer, float volume, u32 nb_floats);
mal_uint32 on_send_frames_to_device(mal_device* pDevice, mal_uint32 frameCount, void* pSamples);

// Private variables 
mal_device_config config;
mal_device device;
mal_context Ctxt;
stb_vorbis* vorbis_file;

bool should_play = false;

#define DJN_MAX_SOUNDS 64
struct 
{
	struct
	{
		float* buffer;
		u32 nb_samples;
		u32 nb_channels;
	}data[DJN_MAX_SOUNDS];
	u32 buffer_cursor = 0;
} djn_audio_sound_data;

#define DJN_MAX_SOUNDS_PLAYING 8
#define DJN_SOUND_NOT_PLAYING ((u32)-1)
struct
{
	struct
	{
		djn_audio_sound	handle;
		u32 sample_pos = DJN_SOUND_NOT_PLAYING;		// if = -1, sample is not playing
	} data[DJN_MAX_SOUNDS_PLAYING];
}	playing_sounds;


void djn_audio_init()
{
    int error = 0;
    vorbis_file = stb_vorbis_open_filename("data/calm.ogg", &error, NULL);

    if (error)
    {
        printf("Error Couldn open vorbis file\n");
        return;
    }
    else
    {
        printf("Vorbis file opened\n");
    }

    if (mal_context_init(NULL, 0, NULL, &Ctxt) != MAL_SUCCESS)
    {
        printf("Couldn't init audio context\n");
        return;
    }

    config = mal_device_config_init(mal_format_f32, 2, SAMPLE_RATE, NULL, &on_send_frames_to_device);

    if (mal_device_init(&Ctxt, mal_device_type_playback,NULL, &config, NULL, &device))
    {
        printf("Couldn't init mal device\n");
        mal_context_uninit(&Ctxt);
        return;
    }

    if (mal_device_start(&device) != MAL_SUCCESS)
    {
        printf("Couldn't start mal device\n");
        mal_device_uninit(&device);
        mal_context_uninit(&Ctxt);
        return;
    }

    should_play = true;

    memset(&djn_audio_sound_data,0, sizeof(djn_audio_sound_data));
}

void djn_audio_deinit()
{
    should_play = false;
    djn_audio_unload_sounds();
    stb_vorbis_close(vorbis_file);
    mal_device_uninit(&device);
}


djn_audio_sound djn_audio_load_sound(const char* path)
{
	// Register the sound
	djn_audio_sound ret_value;
	ret_value.handle = DJN_SOUND_INVALID_HANDLE;

	djn_audio_sound_data.buffer_cursor++;

	unsigned int channels = 2;
	unsigned int sample_rate = SAMPLE_RATE;
	drwav_uint64 nb_samples = 0;
	float* sound = drwav_open_and_read_file_f32(path, &channels, &sample_rate, &nb_samples);

	if (!sound)
	{
		return ret_value;
	}

	if (channels >= 2)
	{
		assert(false && "Too much channels in that wav");
	}

    if (sample_rate != SAMPLE_RATE)
    {
        assert(false && "Not correct sample rate");
    }

	auto& current_sound = djn_audio_sound_data.data[djn_audio_sound_data.buffer_cursor];

	current_sound.buffer 		= sound;
	current_sound.nb_samples 	= nb_samples;
	current_sound.nb_channels 	= channels;

	ret_value.handle = djn_audio_sound_data.buffer_cursor;
	djn_audio_sound_data.buffer_cursor ++;

	return ret_value;
}

void djn_audio_play_sound(djn_audio_sound& to_play)
{
	// Find next empty slot
	int found_sample = 0;
	for (found_sample = 0; found_sample < DJN_MAX_SOUNDS_PLAYING; found_sample++)
	{
		if (playing_sounds.data[found_sample].sample_pos == -1)
		{
			break;
		}
	}

	if (found_sample == DJN_MAX_SOUNDS_PLAYING)
		return;

	playing_sounds.data[found_sample].sample_pos = 0;
	playing_sounds.data[found_sample].handle = to_play;
}

void djn_audio_unload_sounds()
{
	if (djn_audio_sound_data.buffer_cursor == 0)
		return;

	do
	{
		free(djn_audio_sound_data.data[djn_audio_sound_data.buffer_cursor].buffer);
		djn_audio_sound_data.buffer_cursor--;
	}
	while(djn_audio_sound_data.buffer_cursor != 0);

	for (int i = 0; i < DJN_MAX_SOUNDS_PLAYING; i++)
	{
		playing_sounds.data[i].handle = {0};
		playing_sounds.data[i].sample_pos = DJN_SOUND_NOT_PLAYING;
	}
}


mal_uint32 on_send_frames_to_device(mal_device* pDevice, mal_uint32 frameCount, void* pSamples)
{
    if (djn_game_data->is_system_paused || !should_play)
        return 0;
    u32 samples_read = stb_vorbis_get_samples_float_interleaved(vorbis_file, 2, (float*) pSamples, frameCount * 2);
    
    if (samples_read < frameCount)
    {
        stb_vorbis_seek(vorbis_file, 0);
        samples_read += stb_vorbis_get_samples_float_interleaved(vorbis_file, 2, &((float*)pSamples)[samples_read * 2], (frameCount-samples_read) * 2);
    }

    scale_buffer((float*) pSamples, volume, frameCount * 2);

    // Mix

    for (int current_sound = 0; current_sound < DJN_MAX_SOUNDS_PLAYING; current_sound++)
    {
    	auto& sound = playing_sounds.data[current_sound];
    	if (sound.sample_pos != DJN_SOUND_NOT_PLAYING)
    	{
    		auto& data = djn_audio_sound_data.data[sound.handle.handle];
    		float* buffer = data.buffer;
    		float sample_advance = (data.nb_channels/2.0f);

    		for (u32 pos = 0; pos < frameCount*2; pos ++)
    		{
    			u32 buffer_sample_pos = (pos*sample_advance) + sound.sample_pos;
    			if (buffer_sample_pos >= data.nb_samples)
    			{
    				sound.sample_pos = DJN_SOUND_NOT_PLAYING;
    				break;
    			}
    			((float*)pSamples)[pos] += buffer[buffer_sample_pos] * volume;  
    		}

    		if (sound.sample_pos != DJN_SOUND_NOT_PLAYING)
    		{
    			sound.sample_pos += frameCount*2*sample_advance;
    		}
    	}
    }

    // Clamp
    scale_buffer((float*) pSamples, 1.0, frameCount * 2);

    return frameCount;
}

void scale_buffer(float* in_buffer, float volume, u32 nb_floats)
{
	// Overkill optim : Do that with smid ?
    for (u32 cursor = 0; cursor < nb_floats; ++ cursor)
    {
        in_buffer[cursor] *= volume;
    }
}

















#if 0

uint32_t sample_pos = 0;
int32_t pitch_index = 25;
#define SAMPLE_RATE 44100

#define NUM_NOTES 88
static int pitch[NUM_NOTES];


float samples[]
=
{
    1.0f,
    1.0f,
    1.0f,
    1.0f,
    1.0f,
    1.0f,
    1.0f,
    1.0f,
    1.0f,
    1.0f,
    1.0f,
    1.0f,
    1.0f,
    1.0f,
    1.0f,
    1.0f,
    -1.0f,
    -1.0f,
    -1.0f,
    -1.0f,
    -1.0f,
    -1.0f,
    -1.0f,
    -1.0f,
    -1.0f,
    -1.0f,
    -1.0f,
    -1.0f,
    -1.0f,
    -1.0f,
    -1.0f,
    -1.0f
};

float sample_acc = 0.0f;

static double GetNoteSampleRate(int note)
{
	return (pitch[note]) / 44100.0f;
}

static double interpolate(float a, float b, float c)
{
    return a + (b-a) * c;
}


    /*mal_decoder* pDecoder = (mal_decoder*)pDevice->pUserData;
    if (pDecoder == NULL) {
        return 0;
    }

    mal_uint32 samples_read = mal_decoder_read(pDecoder, frameCount, pSamples);

    float* pSamplesFloat = (float*) pSamples;
    for (int i = 0; i < (samples_read*2); i++)
    {
        pSamplesFloat[i] = pSamplesFloat[i] * volume;
    }

    return samples_read;*/

        float sample_speed = GetNoteSampleRate(pitch_index) * 32;

    float* pSamplesFloat = (float*) pSamples;
    for (int i = 0; i < (frameCount*2); i++)
    {
        int current_sample = ((int)sample_acc) % 32;
        int sample_2 = (current_sample + 1) % 32;
        float vol = volume * 0.1f * interpolate(samples[current_sample], samples[sample_2], sample_acc - (int) sample_acc);
        
		pSamplesFloat[i] = vol;

        if ((i % 2) == 1)
        {
            sample_acc += sample_speed;
            if (sample_acc > 32)
            {
                sample_acc -= 32;
            }
        }


    }


static void init_pitch(void)
{
    
    for(int i=0;i<NUM_NOTES;i++)
    {
        pitch[i] = (int)(441.0*(pow(2.0,((i-19.0)/12.0)))); // https://en.wikipedia.org/wiki/Equal_temperament
    }
}
#endif