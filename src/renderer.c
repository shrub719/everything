#include <glad/gl.h>
#include <GLFW/glfw3.h>
#include <stdio.h>
#include "shaders.h"
#include "types.h"

const u16 WIDTH = 200;
const u16 HEIGHT = 100;

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

void r_clear() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

typedef struct {
    GLuint vao;
    GLuint vbo;
    GLuint shader;
} Renderer;

Renderer note_renderer;

void r_init_note_renderer() {
    glGenBuffers(1, &note_renderer.vbo);
    glGenVertexArrays(1, &note_renderer.vao);
    glBindVertexArray(note_renderer.vao);
    glBindBuffer(GL_ARRAY_BUFFER, note_renderer.vbo);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, NULL);
    
    GLuint vs = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vs, 1, &note_vert, NULL);
    glCompileShader(vs);
    handle_shader_error(vs);

    GLuint fs = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fs, 1, &note_frag, NULL);
    glCompileShader(fs);
    handle_shader_error(fs);

    note_renderer.shader = glCreateProgram();
    glAttachShader(note_renderer.shader, vs);
    glAttachShader(note_renderer.shader, fs);
    glBindAttribLocation(note_renderer.shader, 0, "v_pos");
    glLinkProgram(note_renderer.shader);
    handle_program_error(note_renderer.shader);
}

void r_draw_notes() {
    float points[] = {
        0.2, 0.2,
        0.2, -0.2,
        0.2, -0.2,
        -0.2, -0.2,
        -0.2, -0.2,
        -0.2, 0.2,
        -0.2, 0.2,
        0.2, 0.2,
    };

    glUseProgram(note_renderer.shader);
    glBindBuffer(GL_ARRAY_BUFFER, note_renderer.vbo);
    glBindVertexArray(note_renderer.vao);
    glBufferData(GL_ARRAY_BUFFER, 16*sizeof(float), points, GL_STATIC_DRAW);
    glDrawArrays(GL_LINES, 0, 2);
}

