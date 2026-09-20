#include "renderer.h"
#include "window.h"
#include "audio.h"

typedef enum {
    PLAY,   // in a level
    START,  // start screen
    SELECT  // level select
} State;

typedef struct {
    double s;
    double window_cooldown;
} Time;

typedef struct {
    Renderer renderer;
    AudioEngine audio;
    Window window; 
    State state;
    Input input;
    Time time;
} Game;


void game_init(Game* game);
void game_uninit(Game* game);
void game_update(Game* game);
int game_continue(Game* game);

