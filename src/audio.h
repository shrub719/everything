#pragma once
#include "types.h"
#include "miniaudio.h"

typedef struct {
    ma_engine engine;
    ma_sound track;
} Audio;

void au_play_sfx(Audio* audio, u8 id);
void au_play_track(Audio* audio, u8 id);
int au_get_track_ms(Audio* audio);

void au_init(Audio* audio);
void au_uninit(Audio* audio);

