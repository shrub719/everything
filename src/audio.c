#include "types.h"
#define MINIAUDIO_IMPLEMENTATION
#include "miniaudio.h"
#include <stdio.h>
#include "audio.h"

void au_play_sfx(AudioEngine* engine, u8 id) {
    char filename[30];
    sprintf(filename, "assets/sfx_%d.wav", id);
    ma_engine_play_sound(engine, filename, NULL);
}

void au_play_track(AudioEngine* engine, u8 id) {
    char filename[30];
    sprintf(filename, "assets/track_%d.wav", id);
    ma_engine_play_sound(engine, filename, NULL);
}

void au_init(AudioEngine* engine) {
    ma_engine_init(NULL, engine);
}

void au_uninit(AudioEngine* engine) {
    ma_engine_uninit(engine);
}

