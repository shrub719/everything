#pragma once
#include <stdatomic.h>

typedef enum {
    TOP,
    BOTTOM,
    SHADOW_TOP,
    SHADOW_BOTTOM
} Lane;

typedef struct {
    int ms;
    Lane lane;
} Note;

// ATrack is the atomic information intended to be passed to the renderer thread
// seek is the earliest note that should still be processed
typedef struct {
    _Atomic(Note*) seek;
    atomic_int ms;
} ATrack;

typedef struct {
    Note* notes;
    int ms_start;
    ATrack a;
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

