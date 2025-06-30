
#ifndef MOUSE_H
#define MOUSE_H

#include <stdbool.h>


#include <SDL2/SDL_mouse.h>


extern Uint32    mouse_state;

extern SDL_Point mouse_position;

extern Uint32    mouse_global_state;

extern SDL_Point mouse_global_position;

extern Uint32    mouse_relative_state;

extern SDL_Point mouse_relative_position;


void update_mouse(bool enter);

#endif/*MOUSE_H*/
