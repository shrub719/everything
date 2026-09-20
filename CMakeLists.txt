cmake_minimum_required(VERSION 3.20)

project(everything_game C)

set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)

find_package(OpenGL REQUIRED)
find_package(glfw3 CONFIG REQUIRED)
find_package(OpenMP REQUIRED)

add_executable(everything_game 
    src/main.c
    src/audio.c
    src/window.c
    src/input.c
    src/renderer.c
    src/mat.c
)

target_include_directories(everything_game PRIVATE
    include
)

add_subdirectory(glad/cmake glad_cmake)
glad_add_library(glad_gl_core_33 REPRODUCIBLE API gl:core=3.3)

target_link_libraries(everything_game PRIVATE 
    glfw
    glad_gl_core_33
    dl
    pthread
    m
    OpenMP::OpenMP_C
)

add_custom_command(TARGET everything_game POST_BUILD
    COMMAND cmake -E copy_directory
        "${CMAKE_SOURCE_DIR}/assets"
        "$<TARGET_FILE_DIR:everything_game>/assets"
)

