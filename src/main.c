#include "audio.h"
#include "window.h"
#include "renderer.h"
#include "input.h"

int main(void) {
    int result;
    result = win_init();
    if (result != 0) return result;
    result = au_init();
    if (result != 0) {
        win_uninit();
        return result;
    }
    r_init();

    au_play_sfx(0);

    while (win_continue()) {
        // game_update();   // would hopefully include all of the below
        inp_update();
        r_update();
        win_push();
    }

    au_uninit();
    win_uninit();

    return 0;
}

