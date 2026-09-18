#include "types.h"
#define MINIAUDIO_IMPLEMENTATION
#include "miniaudio.h"

ma_engine au_engine;

void au_play_sfx(u8 id) {
    ma_engine_play_sound(&au_engine, "assets/test.wav", NULL);
}

void au_play_track(u8 id) {

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

