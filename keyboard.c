#include "keyboard.h"


const Uint8 *keyboard_state       = NULL;

int keyboard_state_number_of_keys = -1;


void setup_keyboard()
{

    keyboard_state = SDL_GetKeyboardState(&keyboard_state_number_of_keys);

}

void quit_keyboard()
{

    keyboard_state = NULL;

    keyboard_state_number_of_keys = -1;

}
