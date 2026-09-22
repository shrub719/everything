const char* note_vert = 
    "#version 410 core\n"
    "in vec2 a_pos;"
    "in float a_angle;"
    "in float a_x;"
    "in float a_y;"
    "uniform vec2 u_screen_size;"
    "uniform float u_size;"
    "out vec2 v_pos;"
    "void main() {"
    "   float c = cos(a_angle);"
    "   float s = sin(a_angle);"
    "   mat2 rotation = mat2("
    "       c, -s,"
    "       s, c"
    "   );"
    "   v_pos = a_pos;"
    "   vec2 px = rotation * (u_size * a_pos) + vec2(a_x, a_y);"
    "   vec2 coords;"
    "   coords.x = (px.x / u_screen_size.x) * 2.0 - 1.0;"
    "   coords.y = 1.0 - (px.y / u_screen_size.y) * 2.0;"
    "   gl_Position = vec4(coords, 0.0, 1.0);"
    "}";

const char* note_frag =
    "#version 410 core\n"
    "in vec2 v_pos;"
    "out vec4 color;"
    "void main() {"
    "   float border = 0.05;"
    "   if (abs(v_pos.x) > 1.0 - border || abs(v_pos.y) > 1.0 - border) {"
    "       color = vec4(1.0);"
    "   } else {"
    "       color = vec4(0.0, 0.0, 0.0, 1.0);"
    "   }"
    "}";

const char* track_vert = 
    "#version 410 core\n"
    "in vec2 a_px;"
    "uniform vec2 u_screen_size;"
    "void main() {"
    "   vec2 coords;"
    "   coords.x = (a_px.x / u_screen_size.x) * 2.0 - 1.0;"
    "   coords.y = (a_px.y / u_screen_size.y) * 2.0 - 1.0;"
    "   gl_Position = vec4(coords, -1.0, 1.0);"
    "}";

const char* track_frag = 
    "#version 410 core\n"
    "out vec4 color;"
    "void main() {"
    "   color = vec4(1.0);"
    "}";
