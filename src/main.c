#include "audio.h"
#include "window.h"

int main(void) {
    int result;
    result = win_init();
    if (result != 0) return result;
    result = au_init();
    if (result != 0) return result;

    au_play_sfx(0);

    /* Loop until the user closes the window */
    while (win_loop()) {}

    win_uninit();
    au_uninit();

    return 0;
}

