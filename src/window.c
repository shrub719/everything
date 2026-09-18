#include <GLFW/glfw3.h>
#include "types.h"

GLFWwindow* window;

int win_init() {
    if (!glfwInit()) return 1;
    
    window = glfwCreateWindow(640, 480, "EVERYTHING", NULL, NULL);
    if (!window) {
        glfwTerminate();
        return 1;
    }

    glfwMakeContextCurrent(window);

    return 0;
}

void win_uninit() {
    glfwTerminate();
}

int win_loop() {
    glClear(GL_COLOR_BUFFER_BIT);
    glfwSwapBuffers(window);
    glfwPollEvents();
    return !glfwWindowShouldClose(window);
}

