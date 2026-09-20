#include "audio.h"
#include "window.h"
#include "renderer.h"
#include "input.h"

int main(void) {
    int result;
    result = win_init();
    if (result != 0) return result;
    result = au_init();
    if (result != 0) return result;

    au_play_sfx(0);
    r_init_note_renderer();

    while (win_continue()) {
        // game_update();   // would hopefully include all of the below
        inp_update();
        r_clear();
        r_draw_notes();
        win_push();
    }

    win_uninit();
    au_uninit();

    return 0;
}

