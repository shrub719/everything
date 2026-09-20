#define GLAD_GL_IMPLEMENTATION
#include <glad/gl.h>
#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>
#include <stdio.h>
#include "types.h"
#include "input.h"

const u16 INIT_HEIGHT = 480;
const u16 INIT_WIDTH = 960;
const float RATIO = INIT_WIDTH/INIT_HEIGHT;

GLFWwindow* window;

void error_callback(int error, const char* description) {
    fprintf(stderr, "glfw error: code %i\n%s\n", error, description);
}

void frame_buffer_size_callback(GLFWwindow* window, int width, int height) {
    glViewport(0, 0, height * RATIO, height);
}

int win_init() {
    glfwSetErrorCallback(error_callback);

    if (!glfwInit()) return 1;
    
    glfwWindowHint( GLFW_CONTEXT_VERSION_MAJOR, 4 );
    glfwWindowHint( GLFW_CONTEXT_VERSION_MINOR, 1 );
    glfwWindowHint( GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE );
    glfwWindowHint( GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE );

    window = glfwCreateWindow(INIT_WIDTH, INIT_HEIGHT, "EVERYTHING", NULL, NULL);
    if (!window) {
        glfwTerminate();
        return 1;
    }

    glfwMakeContextCurrent(window);
    gladLoadGL(glfwGetProcAddress);

    glfwSwapInterval(0);    // vsync?
    glfwSetKeyCallback(window, inp_callback);
    glfwSetFramebufferSizeCallback(window, frame_buffer_size_callback);

    return 0;
}

void win_uninit() {
    glfwDestroyWindow(window);
    glfwTerminate();
}

const char* vertex_shader =
"#version 410 core\n"
"in vec3 vp;"
"uniform float time;"
"void main() {"
"  vec3 p = vp;"
"  p.x = p.x * cos(time);"
"  gl_Position = vec4(p, 1.0);"
"}";

const char* fragment_shader =
"#version 410 core\n"
"in vec3 color;"
"out vec4 frag_color;"
"void main() {"
"  frag_color = vec4(color, 1.0);"
"}";

/*
void win_loop() {
    float points[] = {
        -0.5f, -0.5f, 0.0f,
        0.5f, -0.5f, 0.0f,
        0.5f, 0.5f, 0.0f,
        -0.5f, 0.5f, 0.0f,
        -0.5f, -0.5f, 0.0f,
    };

    float colors[] = {
        1.0f, 0.0f, 0.0f,
        0.0f, 0.0f, 1.0f,
        0.0f, 1.0f, 0.0f,
        0.0f, 0.0f, 1.0f,
        0.0f, 1.0f, 0.0f,
    };

    GLuint points_vbo = 0;
    glGenBuffers(1, &points_vbo);
    glBindBuffer(GL_ARRAY_BUFFER, points_vbo);
    glBufferData(GL_ARRAY_BUFFER, 15*sizeof(float), points, GL_STATIC_DRAW);

    GLuint colors_vbo = 0;
    glGenBuffers(1, &colors_vbo);
    glBindBuffer(GL_ARRAY_BUFFER, colors_vbo);
    glBufferData(GL_ARRAY_BUFFER, 15*sizeof(float), colors, GL_STATIC_DRAW);

    GLuint vao = 0;
    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);
    glBindBuffer(GL_ARRAY_BUFFER, points_vbo);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, NULL);
    glBindBuffer(GL_ARRAY_BUFFER, colors_vbo);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, NULL);
    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);

    GLuint vs = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vs, 1, &vertex_shader, NULL);
    glCompileShader(vs);
    handle_shader_error(vs);

    GLuint fs = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fs, 1, &fragment_shader, NULL);
    glCompileShader(fs);
    handle_shader_error(fs);

    GLuint shader_program = glCreateProgram();
    glAttachShader(shader_program, fs);
    glAttachShader(shader_program, vs);
    glBindAttribLocation(shader_program, 0, "vp");
    glBindAttribLocation(shader_program, 1, "vc");
    glLinkProgram(shader_program);
    GLint uniform_time = glGetUniformLocation(shader_program, "time");
    handle_program_error(shader_program);

    double prev_s = glfwGetTime();
    double cooldown_s = 0.1f;
    double curr_s, elapsed_s, fps;
    char title[256];

    while (!glfwWindowShouldClose(window)) {
       
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        glUseProgram(shader_program);
        glUniform1f(uniform_time, curr_s);
        glBindVertexArray(vao);
        glDrawArrays(GL_TRIANGLES, 0, 3);

        glfwSwapBuffers(window);
    }
}
*/

int win_continue() {
    return !glfwWindowShouldClose(window);
}

void win_push() {
    glfwSwapBuffers(window);    
}

