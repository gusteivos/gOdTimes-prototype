#include "renderer.h"


bool          renderer_auto_resize = false;

SDL_Rect      renderer_rect        = { 0, 0, 0, 0};

SDL_Renderer *renderer             = NULL;


bool setup_renderer(SDL_Window *window, int index, Uint32 flags)
{

    if (window == NULL)
    {

        fprintf(stderr, "Invalid window to create the renderer.\n");


        return false;

    }


    renderer = SDL_CreateRenderer(window, index, flags);

    if (renderer == NULL)
    {
    
        fprintf(stderr, "Failed to create renderer: %s\n", SDL_GetError());
        

        return false;
    
    }


    return true;

}


void update_renderer_attributes()
{

    SDL_Window *renderer_window = SDL_RenderGetWindow(renderer);

    SDL_GetWindowPosition(renderer_window, &renderer_rect.x, &renderer_rect.y);

    if (renderer_auto_resize)
    {

        SDL_GetWindowSize(renderer_window, &renderer_rect.w, &renderer_rect.h);

    }

    SDL_RenderSetLogicalSize(renderer, renderer_rect.w, renderer_rect.h);

}


void quit_renderer()
{

    renderer_auto_resize = false;

    renderer_rect        = (SDL_Rect){ 0, 0, 0, 0};

    if (renderer != NULL)
    {

        SDL_DestroyRenderer(renderer);
        
        renderer = NULL;
        
    }

}
