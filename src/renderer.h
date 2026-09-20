#include "types.h"
#include <glad/gl.h>

#define WIDTH 960
#define HEIGHT 480

typedef struct {
    u32 buffer[WIDTH * HEIGHT];
    GLuint texture;
    GLuint vao;
    GLuint vbo;
    GLuint shader;
    GLint buffer_uniform;
} Renderer;

void r_init(Renderer* renderer);
void r_update(Renderer* renderer);

