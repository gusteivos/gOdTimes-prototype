
#ifndef KEYBOARD_H
#define KEYBOARD_H

#include <SDL2/SDL_keyboard.h>


extern const Uint8 *keyboard_state;

extern int keyboard_state_number_of_keys;


void setup_keyboard();

void quit_keyboard();

#endif/*KEYBOARD_H*/
