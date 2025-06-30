
#ifndef VIEWPORT_H
#define VIEWPORT_H

#include <stdlib.h>
#include <errno.h>

#include <SDL2/SDL.h>
#include <SDL2/SDL_render.h>


struct viewport_s
{

    SDL_Texture *source;

    float source_aspect;

    SDL_Rect output;

    float output_aspect;

    SDL_Rect scaled;

    float scaled_aspect;

};

typedef struct viewport_s viewport_t;


viewport_t *new_viewport(SDL_Texture *source, SDL_Rect output);

int init_viewport(viewport_t *viewport);

int viewport_update_source_aspect(viewport_t *viewport);

int viewport_update_output_aspect(viewport_t *viewport);

int viewport_update_scaled(viewport_t *viewport);

int viewport_update_scaled_aspect(viewport_t *viewport);

int viewport_render(viewport_t *viewport, SDL_Renderer *renderer);

int viewport_unscaled_coordinates(viewport_t *viewport, int *x, int *y);

void free_viewport(viewport_t *viewport);

#endif/*VIEWPORT_H*/
