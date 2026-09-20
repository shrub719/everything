#include <GLFW/glfw3.h>
#include "audio.h"

void inp_callback(GLFWwindow* window, int key, int scancode, int action, int mods) {
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) {
        glfwSetWindowShouldClose(window, GLFW_TRUE);
    } else if (key == GLFW_KEY_ENTER && action == GLFW_PRESS) {
        au_play_sfx(0);
    }
}

void inp_update() {
    glfwPollEvents();
}

