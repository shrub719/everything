#include <GLFW/glfw3.h>
#include "audio.h"

typedef struct {
    bool keys[GLFW_KEY_LAST + 1];
} Input;
typedef GLFWwindow* Window;

void win_init(Window* window, Input* input);
void win_uninit(Window window);

int win_continue(Window window);
void win_push(Window window);
void win_close(Window window);

double win_get_time();

