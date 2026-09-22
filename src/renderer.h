#pragma once
#include <stdatomic.h>
#include "types.h"
#include <glad/gl.h>
#include "track.h"

#define INIT_WIDTH 960
#define WIDTH INIT_WIDTH*2
#define HEIGHT 480
#define RATIO (float)WIDTH / (float)HEIGHT
#define MS_PER_PIXEL 2
#define PIXELS_PER_MS 1.0/(float)MS_PER_PIXEL
#define NOTE_SIZE 40
#define VERTICAL_PADDING 100
#define HORIZONTAL_PADDING 200
#define FORWARD_TRACK_WIDTH WIDTH + 2 * NOTE_SIZE - HORIZONTAL_PADDING

typedef u32 Color;

typedef struct {
    GLuint vao;
    GLuint vbo;
    GLuint instance_vbo;
    GLuint shader;
    atomic_int* window_height_ptr;
} Renderer;

void r_init(Renderer* renderer);
void r_draw(Renderer* renderer);

