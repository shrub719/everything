cmake_minimum_required(VERSION 3.20)

project(everything_game C)

set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)

find_package(OpenGL REQUIRED)
find_package(glfw3 CONFIG REQUIRED)

add_executable(everything_game 
    src/main.c
    src/audio.c
    src/types.h
)

target_include_directories(everything_game PRIVATE
    include
)

target_link_libraries(everything_game PRIVATE 
    glfw
    OpenGL::GL
    dl
    pthread
    m
)

add_custom_command(TARGET everything_game POST_BUILD
    COMMAND cmake -E copy_directory
        "${CMAKE_SOURCE_DIR}/assets"
        "$<TARGET_FILE_DIR:everything_game>/assets"
)

