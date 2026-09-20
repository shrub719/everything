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
    ma_engine_play_sound(&audio->engine, filename, NULL);
}

void au_init(Audio* audio) {
    ma_engine_init(NULL, &audio->engine);
}

void au_uninit(Audio* audio) {
    ma_engine_uninit(&audio->engine);
}

