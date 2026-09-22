#include <glad/gl.h>
#include <stdatomic.h>
#include <stdio.h>
#include <stdbool.h>
#include <omp.h>
#include <stdlib.h>
#include <stddef.h>
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
    }
}

/*
void draw_note(Renderer* renderer, int ms_until, bool top_lane) {
    int x = HORIZONTAL_PADDING + ms_until * PIXELS_PER_MS;
    int y = top_lane ? VERTICAL_PADDING : HEIGHT-VERTICAL_PADDING;
    int x0 = x - NOTE_SIZE;
    int x1 = x + NOTE_SIZE;
    int y0 = y - NOTE_SIZE;
    int y1 = y + NOTE_SIZE;

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
    draw_line(renderer, 0, 100, 1920, 100, WHITE);
    draw_line(renderer, 0, 380, 1920, 380, WHITE);
    draw_line(renderer, 200, 100, 200, 380, WHITE);

    draw_triangle(renderer, 200, 60, 200, 140, 240, 100, BLACK);
    draw_triangle(renderer, 200, 60, 160, 100, 200, 140, BLACK);
    draw_line(renderer, 200, 60, 240, 100, WHITE);
    draw_line(renderer, 240, 100, 200, 140, WHITE);
    draw_line(renderer, 200, 140, 160, 100, WHITE);
    draw_line(renderer, 160, 100, 200, 60, WHITE);

    draw_triangle(renderer, 200, 340, 200, 420, 240, 380, BLACK);
    draw_triangle(renderer, 200, 340, 160, 380, 200, 420, BLACK);
    draw_line(renderer, 200, 340, 240, 380, WHITE);
    draw_line(renderer, 240, 380, 200, 420, WHITE);
    draw_line(renderer, 200, 420, 160, 380, WHITE);
    draw_line(renderer, 160, 380, 200, 340, WHITE);
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
*/

void r_init_note(InstancedRenderer* note) {
    static const float vertices[] = {
        -1.0, -1.0,
        1.0, -1.0,
        1.0, 1.0,

        -1.0, -1.0,
        1.0, 1.0,
        -1.0, 1.0
    };

    // vao
    glGenVertexArrays(1, &note->vao);
    glBindVertexArray(note->vao);

    // vbo for vertices
    glGenBuffers(1, &note->vbo);
    glBindBuffer(GL_ARRAY_BUFFER, note->vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2*sizeof(float), (void*)0);

    // vbo for instance attributes
    glGenBuffers(1, &note->instance_vbo);
    glBindBuffer(GL_ARRAY_BUFFER, note->instance_vbo);
    glBufferData(GL_ARRAY_BUFFER, 256*sizeof(RNote), NULL, GL_DYNAMIC_DRAW);

    // instance angle
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 1, GL_FLOAT, GL_FALSE, sizeof(RNote), (void*)offsetof(RNote, angle));
    glVertexAttribDivisor(1, 1);

    // instance x
    glEnableVertexAttribArray(2);
    glVertexAttribPointer(2, 1, GL_FLOAT, GL_FALSE, sizeof(RNote), (void*)offsetof(RNote, x));
    glVertexAttribDivisor(2, 1);

    // instance y
    glEnableVertexAttribArray(3);
    glVertexAttribPointer(3, 1, GL_FLOAT, GL_FALSE, sizeof(RNote), (void*)offsetof(RNote, y));
    glVertexAttribDivisor(3, 1);

    GLuint vs = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vs, 1, &note_vert, NULL);
    glCompileShader(vs);
    handle_shader_error(vs);

    GLuint fs = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fs, 1, &note_frag, NULL);
    glCompileShader(fs);
    handle_shader_error(fs);

    note->shader = glCreateProgram();
    glAttachShader(note->shader, vs);
    glAttachShader(note->shader, fs);
    glLinkProgram(note->shader);
    handle_program_error(note->shader);

    glDeleteShader(vs);
    glDeleteShader(fs);

    glUseProgram(note->shader);
    GLint screen_size_uniform = glGetUniformLocation(note->shader, "u_screen_size");
    glUniform2f(screen_size_uniform, (float)WIDTH, (float)HEIGHT);
    GLint size_uniform = glGetUniformLocation(note->shader, "u_size");
    glUniform1f(size_uniform, (float)NOTE_SIZE);

    glBindVertexArray(0);
}

void r_init_track(GenericRenderer* track) {
    static const float vertices[] = {
        0.0, 100.0,     WIDTH, 100.0,
        0.0, 380.0,     WIDTH, 380.0,
        200.0, 0.0,     200.0, HEIGHT
    };

    // vao
    glGenVertexArrays(1, &track->vao);
    glBindVertexArray(track->vao);

    // vbo
    glGenBuffers(1, &track->vbo);
    glBindBuffer(GL_ARRAY_BUFFER, track->vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2*sizeof(float), (void*)0);

    GLuint vs = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vs, 1, &track_vert, NULL);
    glCompileShader(vs);
    handle_shader_error(vs);

    GLuint fs = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fs, 1, &track_frag, NULL);
    glCompileShader(fs);
    handle_shader_error(fs);

    track->shader = glCreateProgram();
    glAttachShader(track->shader, vs);
    glAttachShader(track->shader, fs);
    glLinkProgram(track->shader);
    handle_program_error(track->shader);

    glDeleteShader(vs);
    glDeleteShader(fs);

    glUseProgram(track->shader);
    GLint screen_size_uniform = glGetUniformLocation(track->shader, "u_screen_size");
    glUniform2f(screen_size_uniform, (float)WIDTH, (float)HEIGHT);

    glBindVertexArray(0);
}

void r_init(Renderer* renderer) {
    // don't seem to work
    // glEnable(GL_LINE_SMOOTH);
    // glLineWidth(8.0);
    r_init_note(&renderer->note);
    r_init_track(&renderer->track);
}

void r_draw(Renderer* renderer) {
    static const RNote r_notes[] = {
        { 0.785, 200.0, 100.0 },
        { 0.785, 200.0, 380.0 },
        { 0.2, 100.0, 100.0 },
        { 0.5, 100.0, 100.0 },
        { 0.0, 150.0, 120.0 },
        { 1.0, 200.0, 150.0 }
    };
    
    int height = atomic_load(renderer->window_height_ptr);

    glClear(GL_COLOR_BUFFER_BIT);
    glViewport(0, 0, (float)height * RATIO, height);

    glBindBuffer(GL_ARRAY_BUFFER, renderer->note.instance_vbo);
    glBufferSubData(GL_ARRAY_BUFFER, 0, 6*sizeof(RNote), r_notes);

    glUseProgram(renderer->track.shader);
    glBindVertexArray(renderer->track.vao);
    glDrawArrays(GL_LINES, 0, 6);
    glBindVertexArray(0);

    glUseProgram(renderer->note.shader);
    glBindVertexArray(renderer->note.vao);
    glDrawArraysInstanced(GL_TRIANGLES, 0, 6, 6);   // last number is number of instances
    glBindVertexArray(0);
}

void r_update(Renderer* renderer, Track track) {
    r_draw(renderer);
}

