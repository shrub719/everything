const char* note_vert = 
"#version 410 core\n"
"in vec2 v_pos;"
"void main() {"
"    gl_Position = vec4(v_pos, 0.0, 1.0);"
"}";

const char* note_frag =
"#version 410 core\n"
"out vec4 f_color;"
"void main() {"
"    f_color = vec4(1.0);"
"}";

