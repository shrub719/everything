#include <stdio.h>
#include <stdlib.h>
#include <omp.h>
#include <stdatomic.h>
#include "renderer.h"
#include "window.h"
#include "audio.h"
#include "game.h"

void game_init(Game* game) {
    win_init(&game->window, &game->input);
    au_init(&game->audio);
    // r_init(&game->renderer);
    // done in the thread instead
    inp_init(&game->input);
    game->time.s = win_get_time();
    game->time.window_cooldown = 0.1;
    game->renderer.window_height_ptr = &game->input.window_height;
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

void game_parse_input(Game* game) {     
    inp_update(&game->input);
    if (game->input.hit[GLFW_KEY_ESCAPE]) {
        win_close(game->window);
    } else if (game->input.hit[GLFW_KEY_ENTER]) {
        au_play_sfx(&game->audio, 0);
    }
}

void game_init_track(Game* game) {
    game->state = PLAY;
    au_play_sfx(&game->audio, 0);
    au_play_track(&game->audio, 0);

    game->track.top = malloc(20 * sizeof(Note));
    game->track.bottom = malloc(20 * sizeof(Note));
    game->track.top_seek = game->track.top;
    game->track.bottom_seek = game->track.bottom;
    for (int i = 0; i < 20; i++) {
        game->track.top[i].ms = 500 * i;
        game->track.bottom[i].ms = 250 + 500 * i;
    }
}

void game_uninit_track(Game* game) {
    free(game->track.top);
    free(game->track.bottom);
}

void game_loop(Game* game) {
    game_init_track(game);

    #pragma omp parallel sections
    {
        #pragma omp section
        {
            while (win_continue(game->window)) {
                win_poll(game->window);
                game_parse_input(game);
                game->track.ms = au_get_track_ms(&game->audio);
            }
        }

        #pragma omp section
        {
            win_init_gl(game->window);
            r_init(&game->renderer);
            while (win_continue(game->window)) {
                r_draw(&game->renderer, game->track);
                win_push(game->window);
                game_display_fps(game);
            }
        }
    }

    game_uninit_track(game);
}

