static const char* game_vert = 
    "#version 410 core\n"
    "in vec2 pos;"
    "in vec2 tex_coord;"
    "out vec2 uv;"
    "void main() {"
    "   gl_Position = vec4(pos, 0.0, 1.0);"
    "   uv = tex_coord;"
    "}";

static const char* game_frag =
    "#version 410 core\n"
    "in vec2 uv;"
    "out vec4 color;"
    "uniform sampler2D buffer;"
    "void main() {"
    "   color = texture(buffer, uv);"
    "}";

