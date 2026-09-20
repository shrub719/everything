#include <stdio.h>
#include "renderer.h"
#include "window.h"
#include "audio.h"

typedef enum {
    PLAY,   // in a level
    START,  // start screen
    SELECT  // level select
} State;

typedef struct {
    double s;
    double window_cooldown;
} Time;

typedef struct {
    Renderer renderer;
    AudioEngine audio;
    Window window; 
    State state;
    Input input;
    Time time;
} Game;

void game_init(Game* game) {
    win_init(&game->window, &game->input);
    au_init(&game->audio);
    r_init(&game->renderer);
    au_play_sfx(&game->audio, 0);
    game->time.s = win_get_time();
    game->time.window_cooldown = 0.1;
}

void game_uninit(Game* game) {
    au_uninit(&game->audio);
    win_uninit(game->window);
}

void game_display_fps(Game* game) {
    double curr_s = win_get_time();
    double elapsed_s = curr_s - game->time.s;
    game->time.s = curr_s;
    game->time.window_cooldown -= elapsed_s;
	if (game->time.window_cooldown <= 0.0 && elapsed_s > 0.0) {
		char temp[256];
		sprintf(temp, "EVERYTHING (%.2lf ms, %.2lf fps)", elapsed_s*1000, 1/elapsed_s);
		glfwSetWindowTitle(game->window, temp);
		game->time.window_cooldown = 0.1;
    }
}

void debug(int id) {
    printf("hey %d\n", id);
    fflush(stdout);
}

void game_parse_input(Game* game) {
    debug(game->input.keys[GLFW_KEY_ESCAPE]);
    if (game->input.keys[GLFW_KEY_ESCAPE]) {
        win_close(game->window);
    } else if (game->input.keys[GLFW_KEY_ENTER]) {
        au_play_sfx(&game->audio, 0);
    }
}

void game_update(Game* game) {
    game_parse_input(game);
    r_update(&game->renderer);
    win_push(game->window);
    game_display_fps(game);
}

int game_continue(Game* game) {
    return win_continue(game->window);
}

