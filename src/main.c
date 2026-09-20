#include "game.h"

int main(void) {
    Game game;
    game_init(&game);

    game_loop(&game);

    game_uninit(&game);

    return 0;
}

