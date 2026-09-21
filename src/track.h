#pragma once

typedef struct {
    int ms;
} Note;

typedef struct {
    Note* top;
    Note* bottom;
    Note* top_seek;
    Note* bottom_seek;
    int ms;
} Track;

