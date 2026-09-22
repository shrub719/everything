#pragma once
#include <GLFW/glfw3.h>
#include <stdatomic.h>
#include "audio.h"

typedef struct {
    bool pressed[GLFW_KEY_LAST + 1];
    bool prev_pressed[GLFW_KEY_LAST + 1];
    bool hit[GLFW_KEY_LAST + 1];
    atomic_int window_height;
} Input;
typedef GLFWwindow* Window;

void inp_init(Input* input);
void inp_update(Input* input);

void win_init(Window* window, Input* input);
void win_init_gl(Window window);
void win_uninit(Window window);

void win_poll();
int win_continue(Window window);
void win_push(Window window);
void win_close(Window window);

double win_get_s();
int win_get_ms();

