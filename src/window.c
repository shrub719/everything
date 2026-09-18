#define GLAD_GL_IMPLEMENTATION
#include <glad/gl.h>
#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>
#include <stdio.h>
#include "types.h"
#include "input.h"

const u16 HEIGHT = 480;
const u16 WIDTH = 960;

GLFWwindow* window;

void error_callback(int error, const char* description) {
    fprintf(stderr, "glfw error: code %i\n%s\n", error, description);
}

int win_init() {
    glfwSetErrorCallback(error_callback);

    if (!glfwInit()) return 1;
    
    glfwWindowHint( GLFW_CONTEXT_VERSION_MAJOR, 4 );
    glfwWindowHint( GLFW_CONTEXT_VERSION_MINOR, 1 );
    glfwWindowHint( GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE );
    glfwWindowHint( GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE );

    window = glfwCreateWindow(WIDTH, HEIGHT, "EVERYTHING", NULL, NULL);
    if (!window) {
        glfwTerminate();
        return 1;
    }

    glfwMakeContextCurrent(window);
    gladLoadGL(glfwGetProcAddress);

    glfwSwapInterval(0);    // vsync?
    glfwSetKeyCallback(window, inp_callback);

    return 0;
}

void win_uninit() {
    glfwDestroyWindow(window);
    glfwTerminate();
}

const char* vertex_shader =
"#version 410 core\n"
"in vec3 vp;"
"void main() {"
"  gl_Position = vec4(vp, 1.0);"
"}";

const char* fragment_shader =
"#version 410 core\n"
"in vec3 gl_FragCoord;"
"out vec4 frag_colour;"
"void main() {"
"  frag_colour = vec4(0.0, gl_FragCoord.x/960.0, gl_FragCoord.y/480.0, 1.0);"
"}";

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

void win_setup_level() {
    float points[] = {
        0.0f, 0.5f, 0.0f,
        0.5f, -0.5f, 0.0f,
        -0.5f, -0.5f, 0.0f
    };

    GLuint vbo = 0;
    glGenBuffers(1, &vbo);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, 9*sizeof(float), points, GL_STATIC_DRAW);

    GLuint vao = 0;
    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);
    glEnableVertexAttribArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, NULL);
    int params = -1;

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
    glLinkProgram(shader_program);
    handle_program_error(shader_program);

    int width, height = WIDTH, HEIGHT;
    double prev_s = glfwGetTime();
    double cooldown_s = 0.1f;
    double curr_s, elapsed_s, fps;
    char title[20];

    while (!glfwWindowShouldClose(window)) {
        curr_s = glfwGetTime();
        elapsed_s = curr_s - prev_s;
        prev_s = curr_s;
        cooldown_s -= elapsed_s;
        if (cooldown_s <= 0.0f) {
            fps = 1.0f / elapsed_s;
            sprintf(title, "fps: %.2lf", fps);
            glfwSetWindowTitle(window, title);
            cooldown_s = 0.1f;
        }

        glfwPollEvents();       // do drawing and input in separate threads? is that already the case?
        
        glfwGetWindowSize(window, &width, &height);
        glViewport(0, 0, width, height);

        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        glUseProgram(shader_program);
        glBindVertexArray(vao);
        glDrawArrays(GL_TRIANGLES, 0, 3);

        glfwSwapBuffers(window);
    }
}

