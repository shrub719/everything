#include "audio.h"
#include "window.h"

int main(void) {
    int result;
    result = win_init();
    if (result != 0) return result;
    result = au_init();
    if (result != 0) return result;

    au_play_sfx(0);

    win_setup_level();

    win_uninit();
    au_uninit();

    return 0;
}

