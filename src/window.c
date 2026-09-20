#define GLAD_GL_IMPLEMENTATION
#include <glad/gl.h>
#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>
#include <stdio.h>
#include "types.h"
#include "input.h"

const u16 INIT_HEIGHT = 480;
const u16 INIT_WIDTH = 960;
const float RATIO = (float)INIT_WIDTH / (float)INIT_HEIGHT;

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
int win_continue() {
    return !glfwWindowShouldClose(window);
}

void win_push() {
    glfwSwapBuffers(window);    
}

