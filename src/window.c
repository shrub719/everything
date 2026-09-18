#define GLAD_GL_IMPLEMENTATION
#include <glad/gl.h>
#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>
#include "types.h"
#include "input.h"

const u16 HEIGHT = 480;
const u16 WIDTH = 960;

GLFWwindow* window;

int win_init() {
    if (!glfwInit()) return 1;
    
    window = glfwCreateWindow(WIDTH, HEIGHT, "EVERYTHING", NULL, NULL);
    if (!window) {
        glfwTerminate();
        return 1;
    }

    glfwMakeContextCurrent(window);
    gladLoadGL(glfwGetProcAddress);

    glfwSwapInterval(1);    // vsync?
    glfwSetKeyCallback(window, inp_callback);

    return 0;
}

void win_uninit() {
    glfwDestroyWindow(window);
    glfwTerminate();
}

int win_loop() {
    glClear(GL_COLOR_BUFFER_BIT);
    glfwSwapBuffers(window);
    glfwPollEvents();       // do drawing and input in separate threads? is that already the case?
    return !glfwWindowShouldClose(window);
}

