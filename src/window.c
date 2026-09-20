#define GLAD_GL_IMPLEMENTATION
#include <glad/gl.h>
#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>
#include <stdio.h>
#include "types.h"
#include "audio.h"

const u16 INIT_HEIGHT = 480;
const u16 INIT_WIDTH = 960;
const float RATIO = (float)INIT_WIDTH / (float)INIT_HEIGHT;

typedef GLFWwindow* Window;

void error_callback(int error, const char* description) {
    fprintf(stderr, "glfw error: code %i\n%s\n", error, description);
}

void frame_buffer_size_callback(Window window, int width, int height) {
    glViewport(0, 0, height * RATIO, height);
}

typedef struct {
    bool keys[GLFW_KEY_LAST + 1];
} Input;

void input_callback(Window window, int key, int scancode, int action, int mods) {
    Input* input = (Input *)(glfwGetWindowUserPointer(window));
    if (key >= 0 && key <= GLFW_KEY_LAST) {
        input->keys[key] = (action == GLFW_PRESS);
    }
}

void inp_update(Window window, Input* input, AudioEngine* audio) {
    glfwPollEvents();

    if (input->keys[GLFW_KEY_ESCAPE]) {
        glfwSetWindowShouldClose(window, GLFW_TRUE);
    } else if (input->keys[GLFW_KEY_ENTER]) {
        au_play_sfx(audio, 0);
    }
}

void win_init(Window* window_ptr, Input* input) {
    glfwSetErrorCallback(error_callback);

    glfwInit();
    
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    Window window = glfwCreateWindow(INIT_WIDTH, INIT_HEIGHT, "EVERYTHING", NULL, NULL);
    *window_ptr = window;

    glfwMakeContextCurrent(window);
    gladLoadGL(glfwGetProcAddress);

    glfwSwapInterval(0);    // vsync?
    glfwSetWindowUserPointer(window, input);
    glfwSetKeyCallback(window, input_callback);
    glfwSetFramebufferSizeCallback(window, frame_buffer_size_callback);
}

void win_uninit(Window window) {
    glfwDestroyWindow(window);
    glfwTerminate();
}
int win_continue(Window window) {
    return !glfwWindowShouldClose(window);
}

void win_push(Window window) {
    glfwSwapBuffers(window);    
}

double win_get_time() {
    return glfwGetTime();
}

