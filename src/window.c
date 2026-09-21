#define GLAD_GL_IMPLEMENTATION
#include <glad/gl.h>
#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>
#include <stdatomic.h>
#include <stdio.h>
#include <stdbool.h>
#include "types.h"
#include "audio.h"
#include "renderer.h"
#include "window.h"

void error_callback(int error, const char* description) {
    fprintf(stderr, "glfw error: code %i\n%s\n", error, description);
}

void frame_buffer_size_callback(Window window, int width, int height) {
    Input* input = glfwGetWindowUserPointer(window);
    atomic_store(&input->window_height, height);
}

void inp_callback(Window window, int key, int scancode, int action, int mods) {
    Input* input = glfwGetWindowUserPointer(window);
    if (key >= 0 && key <= GLFW_KEY_LAST) {
        if (action == GLFW_PRESS) {
            input->pressed[key] = true;
        } else if (action == GLFW_RELEASE) {
            input->pressed[key] = false;
        }
    }
}

void inp_init(Input* input) {
    for (int key = 0; key <= GLFW_KEY_LAST; key++) {
        input->pressed[key] = false;
        input->prev_pressed[key] = false;
        input->hit[key] = false;
    }
    input->window_height = HEIGHT;
}

void inp_update(Input* input) {
    for (int key = 0; key <= GLFW_KEY_LAST; key++) {
        input->hit[key] = input->pressed[key] && !(input->prev_pressed[key]);
        input->prev_pressed[key] = input->pressed[key];
    }
}

void win_init(Window* window_ptr, Input* input) {
    glfwSetErrorCallback(error_callback);

    glfwInit();

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    Window window = glfwCreateWindow(INIT_WIDTH, HEIGHT, "EVERYTHING", NULL, NULL);
    *window_ptr = window;

    glfwSetWindowUserPointer(window, input);
    glfwSetKeyCallback(window, inp_callback);
    glfwSetFramebufferSizeCallback(window, frame_buffer_size_callback);
}

void win_init_gl(Window window) {
    glfwMakeContextCurrent(window);
    gladLoadGL(glfwGetProcAddress);
    glfwSwapInterval(0);    // vsync?
}

void win_uninit(Window window) {
    glfwDestroyWindow(window);
    glfwTerminate();
}

void win_poll() {
    glfwPollEvents();
}

int win_continue(Window window) {
    return !glfwWindowShouldClose(window);
}

void win_push(Window window) {
    glfwSwapBuffers(window);    
}

void win_close(Window window) {
    glfwSetWindowShouldClose(window, GLFW_TRUE);
}

double win_get_time() {
    return glfwGetTime();
}

