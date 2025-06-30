
#ifndef MAIN_H_
#define MAIN_H_

#include <assert.h>
#include <stdlib.h>

#include <SDL2/SDL_main.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_events.h>

#include <lua5.3/lua.h>
#include <lua5.3/lauxlib.h>
#include <lua5.3/lualib.h>

#include "window.h"
#include "renderer.h"
#include "viewport.h"
#include "mouse.h"
#include "keyboard.h"
#include "luascript.h"
#include "simulati.h"

#include "gOdtimes.h"


int _error(
    const char *file,
    int line,
    int exit_status,
    const char *fmt,
    ...
    );

#define error(exit_status, fmt, ...) _error(    \
    __FILE__,                                   \
    __LINE__,                                   \
    exit_status,                                \
    fmt,                                        \
    ##__VA_ARGS__                               \
    )


int main(int argc, char *argv[]);

#endif
