#include <glad/gl.h>
#include <stdatomic.h>
#include <stdio.h>
#include <stdbool.h>
#include <omp.h>
#include <stdlib.h>
#include "shaders.h"
#include "types.h"
#include "renderer.h"
#include "track.h"

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

Color rgb(u8 r, u8 g, u8 b) {
    Color color = 0xFF000000;
    color |= r << 16;
    color |= g << 8;
    color |= b << 0;
    return color;
}

#define WHITE rgb(255, 255, 255)
#define BLACK rgb(0, 0, 0)

void clear_buffer(Renderer* renderer) {
    for (int i = 0; i < WIDTH * HEIGHT; i++) {
        // ugh why is anything little endian
        renderer->buffer[i] = 0x00000000;
    }
}

void set_pixel(Renderer* renderer, int x, int y, Color color) {
    renderer->buffer[(y * WIDTH + x) % (WIDTH * HEIGHT)] = color;
}

int barycentric_area(int x0, int y0, int x1, int y1, int x2, int y2) {
    return -((x1 - x0) * (y2 - y0) - (y1 - y0) * (x2 - x0));
}

void draw_line(Renderer* renderer, int x0, int y0, int x1, int y1, Color color) {
    bool steep = abs(x0-x1) < abs(y0-y1);
    if (steep) {
        // should i do fancy xor swap...
        int temp = x0;
        x0 = y0;
        y0 = temp;
        temp = x1;
        x1 = y1;
        y1 = temp;
    }
    if (x0 > x1) {
        int temp = x0;
        x0 = x1;
        x1 = temp;
        temp = y0;
        y0 = y1;
        y1 = temp;
    }
    int y = y0;
    int ierror = 0;
    for (int x = x0; x <= x1; x++) {
        if (steep) {
            set_pixel(renderer, y, x, color);
        } else {
            set_pixel(renderer, x, y, color);
        }
        ierror += 2 * abs(y1-y0);
        if (ierror > x1 - x0) {
            y += y1 > y0 ? 1 : -1;
            ierror -= 2 * (x1 - x0);
        }
    }
}

int min(int a, int b) {
    return a < b ? a : b;
}

int max(int a, int b) {
    return a > b ? a : b;
}

void draw_triangle(Renderer* renderer, int x0, int y0, int x1, int y1, int x2, int y2, Color color) {
    int box_x0 = min(min(x0, x1), x2);
    int box_y0 = min(min(y0, y1), y2);
    int box_x1 = max(max(x0, x1), x2);
    int box_y1 = max(max(y0, y1), y2);
    int area = barycentric_area(x0, y0, x1, y1, x2, y2);
    if (area < 2) return;

    for (int x = box_x0; x <= box_x1; x++) {
        for (int y = box_y0; y <= box_y1; y++) {
            int alpha = barycentric_area(x, y, x1, y1, x2, y2);
            int beta = barycentric_area(x, y, x2, y2, x0, y0);
            int gamma = barycentric_area(x, y, x0, y0, x1, y1);
            if (alpha < 0 || beta < 0 || gamma < 0) continue;
            set_pixel(renderer, x, y, color);
        }
    }
}

void draw_note(Renderer* renderer, int ms_until, bool top_lane) {
    int x = HORIZONTAL_PADDING + ms_until * PIXELS_PER_MS;
    int y = top_lane ? VERTICAL_PADDING : HEIGHT-VERTICAL_PADDING;
    int x0 = x - NOTE_SIZE;
    int x1 = x + NOTE_SIZE;
    int y0 = y - NOTE_SIZE;
    int y1 = y + NOTE_SIZE;
    fflush(stdout);

    draw_triangle(renderer,
            x0, y0,
            x0, y1,
            x1, y1,
            BLACK
    );
    draw_triangle(renderer,
        x0, y0,
        x1, y1,
        x1, y0,
        BLACK
    );
    draw_line(renderer, x0, y0, x0, y1, WHITE);
    draw_line(renderer, x0, y1, x1, y1, WHITE);
    draw_line(renderer, x1, y1, x1, y0, WHITE);
    draw_line(renderer, x1, y0, x0, y0, WHITE);
}

void draw_bg(Renderer* renderer) {
    draw_line(renderer, 0, VERTICAL_PADDING, WIDTH, VERTICAL_PADDING, WHITE);
    draw_line(renderer, 0, HEIGHT-VERTICAL_PADDING, WIDTH, HEIGHT-VERTICAL_PADDING, WHITE);
    draw_line(renderer, HORIZONTAL_PADDING, 0, HORIZONTAL_PADDING, HEIGHT, WHITE);
}

void draw_lane(Renderer* renderer, Note* seek, int ms, bool top_lane) {
    bool too_far = false; 
    // prevent crash when no notes left?
    for (int i = 0; !too_far; i++) {
        int ms_until = seek[i].ms - ms;
        if (ms_until > FORWARD_TRACK_WIDTH * PIXELS_PER_MS) {
            too_far = true;
            continue;
        }

        draw_note(renderer, ms_until, top_lane);
    }
}

void r_draw(Renderer* renderer, Track track) {
    int height = atomic_load(renderer->window_height_ptr);
    clear_buffer(renderer);
    draw_bg(renderer);
    draw_lane(renderer, track.top_seek, track.ms, true);
    draw_lane(renderer, track.bottom_seek, track.ms, false);

    glClear(GL_COLOR_BUFFER_BIT);
    glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, WIDTH, HEIGHT, GL_RGBA, GL_UNSIGNED_BYTE, renderer->buffer);
    glViewport(0, 0, (float)height * RATIO, height);
    glDrawArrays(GL_TRIANGLES, 0, 6);
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

