#include "game.h"

int main(void) {
    Game game;
    game_init(&game);

    while (game_continue(&game)) {
        game_update(&game);
    }

    game_uninit(&game);

    return 0;
}

