#include "types.h"
#define MINIAUDIO_IMPLEMENTATION
#include "miniaudio.h"
#include <stdio.h>
#include "audio.h"

void au_play_sfx(Audio* audio, u8 id) {
    char filename[30];
    sprintf(filename, "assets/sfx_%d.wav", id);
    ma_engine_play_sound(&audio->engine, filename, NULL);
}

void au_play_track(Audio* audio, u8 id) {
    char filename[30];
    sprintf(filename, "assets/track_%d.wav", id);
    ma_sound_init_from_file(&audio->engine, filename, 0, NULL, NULL, &audio->track);
    ma_sound_start(&audio->track);
}

int au_get_track_ms(Audio* audio) {
    ma_uint64 cursor;
    ma_sound_get_cursor_in_pcm_frames(&audio->track, &cursor);

    ma_uint32 sample_rate = ma_engine_get_sample_rate(&audio->engine);

    return (int)((double)cursor * 1000.0 / (double)sample_rate);
}

void au_init(Audio* audio) {
    ma_engine_init(NULL, &audio->engine);
}

void au_uninit(Audio* audio) {
    ma_engine_uninit(&audio->engine);
}

