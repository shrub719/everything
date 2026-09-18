cmake_minimum_required(VERSION 3.20)

project(everythinggame C)

set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)

find_package(OpenGL REQUIRED)
find_package(glfw3 CONFIG REQUIRED)

add_executable(everythinggame src/main.c)

target_link_libraries(everythinggame PRIVATE 
    glfw
    OpenGL::GL
)

