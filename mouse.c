#include "mouse.h"


Uint32    mouse_state = 0x00;

SDL_Point mouse_position = { 0, 0 };

Uint32    mouse_global_state = 0x00;

SDL_Point mouse_global_position = { 0, 0 };

Uint32    mouse_relative_state = 0x00;

SDL_Point mouse_relative_position = { 0, 0 };


void update_mouse(bool enter)
{

    if (enter)
    {

        mouse_state = SDL_GetMouseState(&mouse_position.x, &mouse_position.y);

    }

    mouse_global_state = SDL_GetGlobalMouseState(&mouse_global_position.x, &mouse_global_position.y);

    mouse_relative_state = SDL_GetRelativeMouseState(&mouse_relative_position.x, &mouse_relative_position.y);

}
