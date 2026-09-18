setup:
    rm -r build
    mkdir build
    cmake -S . -B build

build:
    cmake --build build

run:
    ./build/everything_game

[default]
dev: build run
