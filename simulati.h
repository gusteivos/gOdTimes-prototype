
#ifndef SIMULATION_H
#define SIMULATION_H

#include <SDL2/SDL_pixels.h>
#include <SDL2/SDL_render.h>

#include <lua5.3/lua.h>
#include <lua5.3/lauxlib.h>

#include "luascript.h"


#define SIMULATION_PARTICLE_UPDATE_FUNCTION_NAME "update"


struct simulation_particle_data_s
{

    char *name;

    SDL_Color color;

    int lua_ref;

};

typedef struct simulation_particle_data_s simulation_particle_data_t;

simulation_particle_data_t *new_simulation_particle_data(char *name, SDL_Color color, int lua_ref);

void free_simulation_particle_data(simulation_particle_data_t *particle_data);


struct simulation_s
{
    
    lua_Unsigned width;
    
    lua_Unsigned height;
    
    lua_Integer *targets;

    bool *targets_update;
    
    buffer_t *particle_data_buffer;
    
    size_t particle_data_count;
    
    simulation_particle_data_t **particle_data;
    
    lua_State *lua_state;
    
};

typedef struct simulation_s simulation_t;

simulation_t *new_simulation(lua_Unsigned width, lua_Unsigned height, lua_State *lua_state);

int simulation_add_particle_data(simulation_t *simulation, const char *name, SDL_Color color, int lua_ref);

int simulation_get_target(simulation_t *simulation, lua_Unsigned width, lua_Unsigned height, lua_Integer *target);

int simulation_set_target(simulation_t *simulation, lua_Unsigned width, lua_Unsigned height, lua_Integer target);

int simulation_update(simulation_t *simulation);

int simulation_render(simulation_t *simulation, SDL_Renderer *renderer);

void free_simulation(simulation_t *simulation);

int simulation_set_target_update(simulation_t *simulation, lua_Unsigned x, lua_Unsigned y);

#endif/*SIMULATION_H*/
