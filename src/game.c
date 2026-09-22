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
    game->time.s = win_get_s();
    game->time.window_cooldown = 0.1;
    game->renderer.window_height_ptr = &game->input.window_height;
}

void game_uninit(Game* game) {
    au_uninit(&game->audio);
    win_uninit(game->window);
}

void game_display_fps(Game* game) {
    double curr_s = win_get_s();
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
        // temp!!
        Note* seek = atomic_fetch_add(&game->track.a.seek, sizeof(Note));
    }
}

void game_init_track(Game* game) {
    game->state = PLAY;
    au_play_sfx(&game->audio, 0);
    au_play_track(&game->audio, 0);
    game->track.ms_start = win_get_ms();

    game->track.notes = malloc(40 * sizeof(Note));
    atomic_init(&game->track.a.seek, game->track.notes);
    for (int i = 0; i < 20; i++) {
        game->track.notes[2*i].ms = 1000 + 1000 * i;
        game->track.notes[2*i].lane = TOP;
        game->track.notes[2*i + 1].ms = 1500 + 1000 * i;
        game->track.notes[2*i + 1].lane = BOTTOM;
    }
}

void game_uninit_track(Game* game) {
    free(game->track.notes);
}

void game_update_ms(Game* game) {
    // how would i interpolate between the two?
    // int au_ms = au_get_track_ms(&game->audio);
    int win_ms = win_get_ms() - game->track.ms_start;
    int ms = win_ms;
    // int ms = win_ms > au_ms;
    atomic_store(&game->track.a.ms, ms);
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
                game_update_ms(game);
            }
        }

        #pragma omp section
        {
            win_init_gl(game->window);
            r_init(&game->renderer);
            while (win_continue(game->window)) {    // this is race condition
                r_update(&game->renderer, &game->track.a);
                win_push(game->window);
                game_display_fps(game);
            }
            r_uninit(&game->renderer);
        }
    }

    game_uninit_track(game);
}

