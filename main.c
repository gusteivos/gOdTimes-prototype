#include "main.h"


static viewport_t *viewport = NULL;

#define WINDOW_START_WIDTH 640
#define WINDOW_START_HEIGHT 360

#define VIEWPORT_WIDTH 200
#define VIEWPORT_HEIGHT 300


static luascript_pair_descriptor_t gOdTimes_app_descriptor[] = {
    {
        .field = true,
        .type = LUA_TNUMBER,
        .array_table_number_integer = true,
        .name = "width",
        .pairs = NULL
    },
    {
        .field = true,
        .type = LUA_TNUMBER,
        .array_table_number_integer = true,
        .name = "height",
        .pairs = NULL
    },
    {
        .field = true,
        .type = LUA_TBOOLEAN,
        .array_table_number_integer = false,
        .name = "resizable",
        .pairs = NULL
    },
    {
        .field = true,
        .type = LUA_TNUMBER,
        .array_table_number_integer = false,
        .name = "ms",
        .pairs = NULL
    },
    {
        .field = true,
        .type = LUA_TNUMBER,
        .array_table_number_integer = false,
        .name = "mouse_ms",
        .pairs = NULL
    },
    {
        .field = true,
        .type = LUA_TNUMBER,
        .array_table_number_integer = false,
        .name = "keyboard_ms",
        .pairs = NULL
    },
    {
        .field = false,
        .type = LUA_TNONE,
        .array_table_number_integer = false,
        .name = NULL,
        .pairs = NULL
    }
};

static luascript_pair_descriptor_t gOdTimes_descriptor[] = {
    {
        .field = true,
        .type = LUA_TTABLE,
        .array_table_number_integer = false,
        .name = "app",
        .pairs = gOdTimes_app_descriptor
    },
    {
        .field = false,
        .type = LUA_TNONE,
        .array_table_number_integer = false,
        .name = NULL,
        .pairs = NULL
    }
};

struct __attribute__((packed)) gOdTimes_app_s
{
    lua_Unsigned width;
    lua_Unsigned height;
    bool resizable;
    lua_Number ms;
    lua_Number mouse_ms;
    lua_Number keyboard_ms;
};

typedef struct __attribute__((packed)) gOdTimes_app_s gOdTimes_app_t;

struct __attribute__((packed)) gOdTimes_s
{

    gOdTimes_app_t app;

};

typedef struct __attribute__((packed)) gOdTimes_s gOdTimes_t;


void gOdTimes_print(const struct gOdTimes_s *gOdTimes)
{
    if (!gOdTimes)
    {
        printf("gOdTimes is NULL\n");
        return;
    }

    printf("gOdTimes.app:\n");
    printf("  width     = %lld\n", (long long)gOdTimes->app.width);
    printf("  height    = %lld\n", (long long)gOdTimes->app.height);
    printf("  resizable = %s\n", gOdTimes->app.resizable ? "true" : "false");
    printf("  ms        = %lld\n", (long long)gOdTimes->app.ms);
}

gOdTimes_t config = { 0 };


static char *shift_argcv(int *argc, char **argv[])
{

    assert(argc != NULL && argv != NULL);
    assert(*argc > 0 && *argv != NULL);

    char *arg = (*argv)[0];

    --(*argc);
    ++(*argv);

    return arg;

}

static void usage(FILE *out, const char *arg0, bool show_help, int exit_status)
{

    fprintf(
        out,
        "\n"
        "Usage: %s\n"
        "\n",
        arg0
        );

if (show_help)
{

    fprintf(
        out,
        "\t-h\n"
        );

}

    fputc('\n', out);

    fflush(out);

    exit(exit_status);

}

static void version(FILE *out, bool all, int exit_code)
{

    if (all)
    {

        SDL_version SDL2_compiled;
        SDL_VERSION(&SDL2_compiled);
        SDL_version SDL2_linked;
        SDL_GetVersion(&SDL2_linked);
        fprintf(
            out,
            "SDL2 version - compiled: %d.%d.%d, linked: %d.%d.%d\n",
            SDL2_compiled.major,
            SDL2_compiled.minor,
            SDL2_compiled.patch,
            SDL2_linked.major,
            SDL2_linked.minor,
            SDL2_linked.patch
        );

        lua_State *lua_linked_state = luaL_newstate();
        const lua_Number *lua_linked_version = lua_version(lua_linked_state);
        fprintf(
            out,
            "Lua version - compiled: %d.%s, linked: %.1f\n",
            LUA_VERSION_NUM, LUA_VERSION_RELEASE,
            *lua_linked_version
        );
        lua_close(lua_linked_state);

    }

    fprintf(
        out,
        "gOdTimes version - %d.%d.%d\n",
        GODTIMES_VERSION_MAJOR, GODTIMES_VERSION_MINOR, GODTIMES_VERSION_PATCH
    );

    fflush(out);

    exit(exit_code);

}

int
SDL_RenderDrawCircle(SDL_Renderer * renderer, int x, int y, int radius)
{
    int offsetx, offsety, d;
    int status;

    offsetx = 0;
    offsety = radius;
    d = radius -1;
    status = 0;

    while (offsety >= offsetx) {
        status += SDL_RenderDrawPoint(renderer, x + offsetx, y + offsety);
        status += SDL_RenderDrawPoint(renderer, x + offsety, y + offsetx);
        status += SDL_RenderDrawPoint(renderer, x - offsetx, y + offsety);
        status += SDL_RenderDrawPoint(renderer, x - offsety, y + offsetx);
        status += SDL_RenderDrawPoint(renderer, x + offsetx, y - offsety);
        status += SDL_RenderDrawPoint(renderer, x + offsety, y - offsetx);
        status += SDL_RenderDrawPoint(renderer, x - offsetx, y - offsety);
        status += SDL_RenderDrawPoint(renderer, x - offsety, y - offsetx);

        if (status < 0) {
            status = -1;
            break;
        }

        if (d >= 2*offsetx) {
            d -= 2*offsetx + 1;
            offsetx +=1;
        }
        else if (d < 2 * (radius - offsety)) {
            d += 2 * offsety - 1;
            offsety -= 1;
        }
        else {
            d += 2 * (offsety - offsetx - 1);
            offsety -= 1;
            offsetx += 1;
        }
    }

    return status;
}

int
SetCircle(simulation_t *simul, int x, int y, int radius, lua_Integer q)
{
    int offsetx, offsety, d;
    int status;

    offsetx = 0;
    offsety = radius;
    d = radius -1;
    status = 0;

    while (offsety >= offsetx) {

        status += simulation_set_target(simul, x + offsetx, y + offsety, q);
        status += simulation_set_target(simul, x + offsety, y + offsetx, q);
        status += simulation_set_target(simul, x - offsetx, y + offsety, q);
        status += simulation_set_target(simul, x - offsety, y + offsetx, q);
        status += simulation_set_target(simul, x + offsetx, y - offsety, q);
        status += simulation_set_target(simul, x + offsety, y - offsetx, q);
        status += simulation_set_target(simul, x - offsetx, y - offsety, q);
        status += simulation_set_target(simul, x - offsety, y - offsetx, q);

        if (status < 0) {
            status = -1;
            break;
        }

        if (d >= 2*offsetx) {
            d -= 2*offsetx + 1;
            offsetx +=1;
        }
        else if (d < 2 * (radius - offsety)) {
            d += 2 * offsety - 1;
            offsety -= 1;
        }
        else {
            d += 2 * (offsety - offsetx - 1);
            offsety -= 1;
            offsetx += 1;
        }
    }

    return status;
}


int main(int argc, char *argv[])
{

    const char *arg0 = (const char *)shift_argcv(&argc, &argv);

    while (argc > 0)
    {

        const char *arg = (const char *)shift_argcv(&argc, &argv);

        if      (strcmp(arg, "-v") == 0)
        {

            version (stdout,    true,           EXIT_SUCCESS);

        }
        else if (strcmp(arg, "-h") == 0)
        {

            usage   (stdout,    arg0, false,    EXIT_SUCCESS);

        }
        else
        {

            usage   (stderr,    arg0, true,     EXIT_FAILURE);

        }

    }


    int SDL_Init_return = SDL_Init(SDL_INIT_VIDEO | SDL_INIT_EVENTS);

    if (SDL_Init_return < 0)
    {

        error(
            EXIT_FAILURE,
            "SDL_Init returned %d and SDL_GetError \"%s\".",
            SDL_Init_return,
            SDL_GetError()
        );

    }

    atexit(
        SDL_Quit
    );


    lua_State *lua_config_state = luaL_newstate();
    int w;
    int q = luascript_do_file(lua_config_state, "config.lua", &w);
    luascript_error_message(lua_config_state, q, true);
    luascript_error_message(lua_config_state, w, true);

    lua_getglobal(lua_config_state, "gOdTimes");

    buffer_t *config_buffer = new_buffer(sizeof(void *));

    luascript_serialize_pairs(lua_config_state, gOdTimes_descriptor, config_buffer, NULL, NULL);

    config.app = *(gOdTimes_app_t *)config_buffer->items;

    gOdTimes_print(&config);

    lua_pop(lua_config_state, 1);

    lua_State *lua_simulation_state = luaL_newstate();

    luaL_openlibs(lua_simulation_state);

    simulation_t *simulation = new_simulation(
        VIEWPORT_WIDTH,
        VIEWPORT_HEIGHT,
        lua_simulation_state
    );

    q = luascript_do_file(lua_simulation_state, "logic.lua", &w);
    luascript_error_message(lua_simulation_state, q, true);
    luascript_error_message(lua_simulation_state, w, true);

    setup_window(
        "gOd times",
        config.app.width,
        config.app.height,
        SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN
    );

    SDL_SetWindowResizable(window, (SDL_bool)config.app.resizable);

    setup_renderer(window, -1, SDL_RENDERER_ACCELERATED);

    viewport = new_viewport(
        SDL_CreateTexture(
            renderer,
            SDL_PIXELFORMAT_RGBA32,
            SDL_TEXTUREACCESS_TARGET,
            VIEWPORT_WIDTH,
            VIEWPORT_HEIGHT
        ),
        (SDL_Rect){ 0, 0, config.app.width, config.app.height }
    );

    setup_keyboard();

    lua_Integer left = 0;
    lua_Integer right = 0;

    Uint32 mouse_time;
    Uint32 keyboard_time;

    loop_start();

    loop_set_target_elapsed_performance_counter_fms(config.app.ms);

    while (1)
    {

        loop_on_start();

        update_window_attributes();

        SDL_Event event;

        while (SDL_PollEvent(&event) == 1)
        {

            switch (event.type)
            {

            case SDL_QUIT:
                return EXIT_SUCCESS;
                break;

            case SDL_WINDOWEVENT:
                {

                    if (event.window.windowID == SDL_GetWindowID(window))
                    {

                        handle_window(&event.window);

                        viewport->output = window_rect;

                        viewport_update_output_aspect(viewport);

                        viewport_update_scaled(viewport);

                    }
                    else
                    {

                        return EXIT_FAILURE;

                    }

                }
                break;

            }

        }

        printf("loop_fdelta_time %f, fps %f.\n", loop_fdelta_time, 1 / loop_fdelta_time);

        loop_on_middle();

        keyboard_time += loop_u64delta_time;

        if (keyboard_time >= config.app.keyboard_ms)
        {

            if (window_focus_gained)
            {

                if (keyboard_state[SDL_SCANCODE_U])
                {
                    --left;
                }
                if (keyboard_state[SDL_SCANCODE_I])
                {
                    ++left;
                }
                if (keyboard_state[SDL_SCANCODE_J])
                {
                    --right;
                }
                if (keyboard_state[SDL_SCANCODE_K])
                {
                    ++right;
                }

            }

            keyboard_time = 0;

        }

        mouse_time += loop_u64delta_time;

        if (mouse_time > config.app.mouse_ms)
        {

            update_mouse(window_enter);
            
            SDL_Point mouse_position_viewport = mouse_position;

            viewport_unscaled_coordinates(viewport, &mouse_position_viewport.x, &mouse_position_viewport.y);

            if (window_enter)
            {

                if (mouse_state & SDL_BUTTON(SDL_BUTTON_LEFT))
                {

                    SetCircle(simulation, mouse_position_viewport.x, mouse_position_viewport.y, 2, left);

                }

                if (mouse_state & SDL_BUTTON(SDL_BUTTON_RIGHT))
                {

                    simulation_set_target(simulation, mouse_position_viewport.x, mouse_position_viewport.y, right);

                }

            }

            mouse_time = 0;

        }


        char title[128];
        snprintf(title, sizeof(title), "gOdTimes [left %lld | right %lld]", (long long)left, (long long)right);
        SDL_SetWindowTitle(window, title);


        simulation_update(simulation);


        SDL_SetRenderDrawColor(renderer, 127, 127, 127, 255);

        SDL_RenderClear(renderer);


        SDL_SetRenderTarget(renderer, viewport->source);

        SDL_SetRenderDrawColor(renderer, 255, 0, 127, 255);

        SDL_RenderClear(renderer);

        simulation_render(simulation, renderer);


        update_mouse(window_enter);
        
        SDL_Point mouse_position_viewport = mouse_position;

        viewport_unscaled_coordinates(viewport, &mouse_position_viewport.x, &mouse_position_viewport.y);


        SDL_SetRenderDrawColor(renderer, 127, 0, 0, 127);

        SDL_RenderDrawPoint(renderer, mouse_position_viewport.x, mouse_position_viewport.y);

        SDL_SetRenderDrawColor(renderer, 0, 127, 0, 127);

        SDL_RenderDrawCircle(renderer, mouse_position_viewport.x, mouse_position_viewport.y, 3);

        SDL_SetRenderDrawColor(renderer, 0, 0, 127, 127);

        SDL_RenderDrawCircle(renderer, mouse_position_viewport.x, mouse_position_viewport.y, 5);


        SDL_SetRenderTarget(renderer, NULL);

        viewport_render(viewport, renderer);

        SDL_RenderPresent(renderer);

        loop_on_end();

    }

    loop_end();

    return EXIT_SUCCESS;

}


int _error(
    const char *file,
    int line,
    int exit_status,
    const char *fmt,
    ...
    )
{

    fprintf(stderr, "\nError from FILE: %s on LINE: %d.\n\n", file, line);

	va_list ap;
	va_start(ap, fmt);
	vfprintf(stderr, fmt, ap);
	va_end(ap);

	fputs("\n\n", stderr);

	fflush(stderr);

    exit(exit_status);

}
