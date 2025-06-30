#include "viewport.h"


viewport_t *new_viewport(SDL_Texture *source, SDL_Rect output)
{

    if (source == NULL)
    {

        return NULL;

    }

    viewport_t *viewport = (viewport_t *)malloc(sizeof(viewport_t));

    if (viewport == NULL)
    {

        return NULL;

    }

    viewport->source = source;

    viewport->output = output;

    int init_viewport_return = init_viewport(viewport);

    /*
        TODO: handle init_viewport_return error.
    */

    return viewport;

}

int init_viewport(viewport_t *viewport)
{

    if (viewport == NULL)
    {

        return EINVAL;

    }

    viewport_update_source_aspect(viewport);

    viewport_update_output_aspect(viewport);

    viewport_update_scaled(viewport);

    viewport_update_scaled_aspect(viewport);

    return 0;

}

int viewport_update_source_aspect(viewport_t *viewport)
{

    if (viewport == NULL)
    {

        return EINVAL;

    }

    int w;

    int h;

    int SDL_QueryTexture_return = SDL_QueryTexture(viewport->source, NULL, NULL, &w, &h);

    /*
        TODO: handle SDL_QueryTexture error.
    */

    viewport->source_aspect = (float)w / (float)h;

    return 0;

}

int viewport_update_output_aspect(viewport_t *viewport)
{

    if (viewport == NULL)
    {

        return EINVAL;

    }

    viewport->output.w  = (viewport->output.w == 0) ? 1 : viewport->output.w;

    viewport->output.h  = (viewport->output.h == 0) ? 1 : viewport->output.h;

    viewport->output_aspect = (float)viewport->output.w / (float)viewport->output.h;

    return 0;

}

int viewport_update_scaled(viewport_t *viewport)
{

    if (viewport == NULL)
    {

        return EINVAL;

    }

    if (viewport->output_aspect > viewport->source_aspect)
    {

        viewport->scaled.w = viewport->output.h * viewport->source_aspect;

        viewport->scaled.h = viewport->output.h;

        viewport->scaled.x = (viewport->output.w - viewport->scaled.w) / 2;

        viewport->scaled.y = 0;

    }
    else
    {

        viewport->scaled.w = viewport->output.w;

        viewport->scaled.h = viewport->output.w / viewport->source_aspect;

        viewport->scaled.x = 0;

        viewport->scaled.y = (viewport->output.h - viewport->scaled.h) / 2;

    }

    return 0;

}

int viewport_update_scaled_aspect(viewport_t *viewport)
{

    if (viewport == NULL)
    {

        return EINVAL;

    }

    viewport->scaled.w  = (viewport->scaled.w == 0) ? 1 : viewport->scaled.w;

    viewport->scaled.h  = (viewport->scaled.h == 0) ? 1 : viewport->scaled.h;

    viewport->scaled_aspect = (float)viewport->scaled.w / (float)viewport->scaled.h;

    return 0;

}

int viewport_render(viewport_t *viewport, SDL_Renderer *renderer)
{

    if (viewport == NULL || renderer == NULL)
    {

        return EINVAL;

    }

    /*
        TODO: validate viewport elements.
    */

    int SDL_RenderCopy_return = SDL_RenderCopy(renderer, viewport->source, NULL, &viewport->scaled);

    /*
        TODO: handle SDL_RenderCopy error.
    */

    return 0;

}

int viewport_unscaled_coordinates(viewport_t *viewport, int *x, int *y)
{

    if (viewport == NULL)
    {

        return EINVAL;

    }

    int w;

    int h;

    int SDL_QueryTexture_return = SDL_QueryTexture(viewport->source, NULL, NULL, &w, &h);

    /*
        TODO: handle SDL_QueryTexture error.
    */

    if (x != NULL)
    {

        *x -= viewport->scaled.x;

        *x = (float)*x * ((float)w / (float)viewport->scaled.w);

    }

    if (y != NULL)
    {

        *y -= viewport->scaled.y;

        *y = (float)*y * ((float)h / (float)viewport->scaled.h);

    }

    return 0;

}

void free_viewport(viewport_t *viewport)
{

    if (viewport == NULL)
    {

        return;

    }

    if (viewport->source != NULL)
    {

        SDL_DestroyTexture(viewport->source);

    }

    free((void *)viewport);

}
