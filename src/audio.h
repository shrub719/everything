#include "types.h"
#include "miniaudio.h"

typedef ma_engine AudioEngine;

void au_play_sfx(AudioEngine* engine, u8 id);
void au_play_track(AudioEngine* engine, u8 id);

int au_init(AudioEngine* engine);
void au_uninit(AudioEngine* engine);

