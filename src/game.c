#include "renderer.h"
#include "window.h"
#include "audio.h"

typedef enum {
    PLAY,   // in a level
    START,  // start screen
    SELECT  // level select
} State;

typedef struct {
    Renderer renderer;
    AudioEngine audio;
    Window window; 
    State state;
    Input input;
} Game;

void game_init(Game* game) {
    win_init(&game->window, &game->input);
    au_init(&game->audio);
    r_init(&game->renderer);
    au_play_sfx(&game->audio, 0);
}

void game_uninit(Game* game) {
    au_uninit(&game->audio);
    win_uninit(game->window);
}

void game_update(Game* game) {
    inp_update(game->window, &game->input, &game->audio);
    r_update(&game->renderer);
    win_push(game->window);
}

int game_continue(Game* game) {
    return win_continue(game->window);
}

