#include <glad/gl.h>
#include <stdio.h>
#include <omp.h>
#include "shaders.h"
#include "types.h"

void handle_shader_error(GLuint index) {
    int params = -1;
    glGetShaderiv(index, GL_COMPILE_STATUS, &params);
    if (GL_TRUE != params) {
        int max_length = 2048;
        int actual_length = 0;
        char slog[2048];
        glGetShaderInfoLog(index, max_length, &actual_length, slog);
        fprintf(stderr, "shader error: shader index %u did not compile\n%s\n", index, slog);
        // return 1;
    }
}

void handle_program_error(GLuint index) {
    int params = -1;
    glGetProgramiv(index, GL_LINK_STATUS, &params);
    if ( GL_TRUE != params ) {    
        int max_length = 2048;
        int actual_length = 0;
        char plog[2048];
        glGetProgramInfoLog(index, max_length, &actual_length, plog);
        fprintf( stderr, "shader error: could not link shader index %u\n%s\n", index, plog );
        // return 1;
    }
}

#define WIDTH 960
#define HEIGHT 480

typedef struct {
    u32 buffer[WIDTH * HEIGHT];
    GLuint texture;
    GLuint vao;
    GLuint vbo;
    GLuint shader;
    GLint buffer_uniform;
} Renderer;

void clear_buffer(Renderer* renderer) {
    #pragma omp parallel for
    for (int i = 0; i < WIDTH * HEIGHT; i++) {
        renderer->buffer[i] = 0xFF0000FF;
    }
}

void draw_notes(Renderer* renderer) {
    for (int x = 0; x < WIDTH; x++) {
        renderer->buffer[200 * WIDTH + x] = 0xFFFFFFFF;
    }
}

void r_init(Renderer* renderer) {
    glGenTextures(1, &renderer->texture);
    glBindTexture(GL_TEXTURE_2D, renderer->texture);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, WIDTH, HEIGHT, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);
    glBindTexture(GL_TEXTURE_2D, 0);

    float vertices[] = {
        -1.0, 1.0,      0.0, 0.0,
        -1.0, -1.0,     0.0, 1.0,
        1.0, -1.0,      1.0, 1.0,

        -1.0, 1.0,      0.0, 0.0,
        1.0, -1.0,      1.0, 1.0,
        1.0, 1.0,       1.0, 0.0
    };

    glGenVertexArrays(1, &renderer->vao);
    glGenBuffers(1, &renderer->vbo);
    glBindVertexArray(renderer->vao);
    glBindBuffer(GL_ARRAY_BUFFER, renderer->vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4*sizeof(float), (void *)(0));
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4*sizeof(float), (void *)(2 * sizeof(float)));
    glBindVertexArray(0);

    GLuint vs = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vs, 1, &game_vert, NULL);
    glCompileShader(vs);
    handle_shader_error(vs);

    GLuint fs = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fs, 1, &game_frag, NULL);
    glCompileShader(fs);
    handle_shader_error(fs);

    renderer->shader = glCreateProgram();
    glAttachShader(renderer->shader, vs);
    glAttachShader(renderer->shader, fs);
    glLinkProgram(renderer->shader);
    handle_program_error(renderer->shader);

    glDeleteShader(vs);
    glDeleteShader(fs);

    renderer->buffer_uniform = glGetUniformLocation(renderer->shader, "buffer");

    glUseProgram(renderer->shader);
    glBindTexture(GL_TEXTURE_2D, renderer->texture);
    glActiveTexture(GL_TEXTURE0);
    glUniform1i(renderer->buffer_uniform, 0);
    glBindVertexArray(renderer->vao);
}

void r_update(Renderer* renderer) {
    clear_buffer(renderer);
    draw_notes(renderer);

    glClear(GL_COLOR_BUFFER_BIT);
    glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, WIDTH, HEIGHT, GL_RGBA, GL_UNSIGNED_BYTE, renderer->buffer);
    glDrawArrays(GL_TRIANGLES, 0, 6);
}

