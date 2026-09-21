#pragma once
#include <stdatomic.h>
#include "types.h"
#include <glad/gl.h>

#define INIT_WIDTH 960
#define WIDTH INIT_WIDTH*2
#define HEIGHT 480
static const float RATIO = (float)WIDTH / (float)HEIGHT;

typedef u32 Color;

typedef struct {
    u32 buffer[WIDTH * HEIGHT];
    GLuint texture;
    GLuint vao;
    GLuint vbo;
    GLuint shader;
    GLint buffer_uniform;
    atomic_int* window_height_ptr;
} Renderer;

void r_init(Renderer* renderer);
void r_update(Renderer* renderer);

