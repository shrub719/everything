const char* game_vert = 
    "#version 410 core\n"
    "in vec2 a_pos;"
    "in float a_angle;"
    "in float a_x;"
    "in float a_y;"
    "uniform vec2 u_screen_size;"
    "void main() {"
    "   float c = cos(a_angle);"
    "   float s = sin(a_angle);"
    "   mat2 rotation = mat2("
    "       c, -s,"
    "       s, c"
    "   );"
    "   vec2 px = rotation * a_pos + vec2(a_x, a_y);"
    "   vec2 coords;"
    "   coords.x = (px.x / u_screen_size.x) * 2.0 - 1.0;"
    "   coords.y = (px.y / u_screen_size.y) * 2.0 - 1.0;"
    "   gl_Position = vec4(coords, 0.0, 1.0);"
    "}";

const char* game_frag =
    "#version 410 core\n"
    "out vec4 color;"
    "void main() {"
    "   color = vec4(1.0);"
    "}";

