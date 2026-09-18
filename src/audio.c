#include "types.h"
#define MINIAUDIO_IMPLEMENTATION
#include "miniaudio.h"
#include <stdio.h>

ma_engine au_engine;

void au_play_sfx(u8 id) {
    char filename[21];
    sprintf(filename, "assets/sfx_%d.wav", id);
    ma_engine_play_sound(&au_engine, filename, NULL);
}

void au_play_track(u8 id) {
    char filename[21];
    sprintf(filename, "assets/track_%d.wav", id);
    ma_engine_play_sound(&au_engine, filename, NULL);
}

int au_init() {
    ma_result result;
    result = ma_engine_init(NULL, &au_engine);

    if (result != MA_SUCCESS) {
        return 1;
    }
    
    return 0;
}

void au_uninit() {
    ma_engine_uninit(&au_engine);
}

