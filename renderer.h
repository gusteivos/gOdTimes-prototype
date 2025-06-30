
#ifndef RENDERER_H
#define RENDERER_H

#include <stdbool.h>
#include <stdio.h>

#include <SDL2/SDL.h>


extern bool          renderer_auto_resize;

extern SDL_Rect      renderer_rect;

extern SDL_Renderer *renderer;


bool setup_renderer(SDL_Window *window, int index, Uint32 flags);

void update_renderer_attributes(void);

void quit_renderer(void);

#endif/*RENDERER_H*/
