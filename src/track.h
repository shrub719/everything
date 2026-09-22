#pragma once
#include <stdatomic.h>

typedef struct {
    int ms;
} Note;

// seek is the earliest note that should still be processed
typedef struct {
    Note* top;
    Note* bottom;
    _Atomic(Note*) top_seek;
    _Atomic(Note*) bottom_seek;
    int ms;
} Track;

// produced by renderer deciding which notes to draw
// contains info relevant to vert shader
typedef struct {
    float angle;
    float x;
    float y;
} RNote;

/*
// do i need to have separate trace objects?? really??
// trace here being a fancy word for the note outlines
// no, i don't
typedef struct {
    float angle;
    float x;
    float y;
    float decay;
} RTrace;
*/

