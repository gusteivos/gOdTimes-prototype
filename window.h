
#ifndef WINDOW_H
#define WINDOW_H

#include <stdbool.h>
#include <stdio.h>

#include <SDL2/SDL.h>
#include <SDL2/SDL_syswm.h>


extern Uint32        window_flags;

extern SDL_Rect      window_rect;

extern SDL_Window   *window;

extern SDL_SysWMinfo window_sysinfo;

extern bool window_enter;

extern bool window_focus_gained;


bool setup_window(const char *title, int w, int h, Uint32 flags);

void update_window_attributes(void);

void handle_window(SDL_WindowEvent *window_event);

void quit_window(void);

#endif/*WINDOW_H*/
