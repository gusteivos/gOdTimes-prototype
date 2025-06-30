#include "window.h"


Uint32        window_flags   = 0x00;

SDL_Rect      window_rect    = { 0, 0, 0, 0 };

SDL_Window   *window         = NULL;

SDL_SysWMinfo window_sysinfo = { 0 };

bool window_enter            = false;

bool window_focus_gained     = false;



bool setup_window(const char *title, int w, int h, Uint32 flags)
{

    if ((window = SDL_CreateWindow(title, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, w, h, flags)) == NULL)
    {

        fprintf(stderr, "Failed to create window: %s\n", SDL_GetError());


        return false;

    }

    update_window_attributes();


    SDL_VERSION(&window_sysinfo.version);

    if (SDL_GetWindowWMInfo(window, &window_sysinfo) == SDL_FALSE)
    {

        fprintf(stderr, "Failed to get sysinfo from window: %s\n", SDL_GetError());


        return false;

    }


    return true;

}


void update_window_attributes()
{

    window_flags = SDL_GetWindowFlags(window);

    SDL_GetWindowPosition(window, &window_rect.x, &window_rect.y);

    SDL_GetWindowSize    (window, &window_rect.w, &window_rect.h);

}

void handle_window(SDL_WindowEvent *window_event)
{

    switch (window_event->event)
    {

    case SDL_WINDOWEVENT_MOVED:

        window_rect.x = window_event->data1;

        window_rect.y = window_event->data2;

        break;

    case SDL_WINDOWEVENT_SIZE_CHANGED:

        window_rect.w = window_event->data1;

        window_rect.h = window_event->data2;

        break;

    case SDL_WINDOWEVENT_ENTER:
        window_enter = true;
        break;

    case SDL_WINDOWEVENT_LEAVE:
        window_enter = false;
        break;

    case SDL_WINDOWEVENT_FOCUS_GAINED:
        window_focus_gained = true;
        break;

    case SDL_WINDOWEVENT_FOCUS_LOST:
        window_focus_gained = false;
        break;

    default:

#ifndef NDEBUG

        fprintf(stderr, "window event not handled: %d.\n", window_event->event);

#endif

        break;

    }

}


void quit_window()
{

    window_flags = 0x00;

    window_rect  = (SDL_Rect){ 0, 0, 0, 0 };

    if (window != NULL)
    {

        SDL_DestroyWindow(window);

        window = NULL;

    }

    window_sysinfo = (SDL_SysWMinfo){ 0 };

}
