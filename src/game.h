#include "renderer.h"
#include "window.h"
#include "audio.h"

typedef enum {
    PLAY,   // in a level
    START,  // start screen
    SELECT  // level select
} State;

typedef struct {
    Renderer renderer;
    AudioEngine audio;
    Window window; 
    State state;
} Game;


void game_init(Game* game);
void game_uninit(Game* game);
void game_update(Game* game);
int game_continue(Game* game);

