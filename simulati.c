#include "simulati.h"


static int lua_simulation_get_width(lua_State *L)
{
    simulation_t *sim = luaL_checkudata(L, 1, "Simulation");
    lua_pushinteger(L, (lua_Integer)(sim->width));
    return 1;
}

static int lua_simulation_get_height(lua_State *L)
{
    simulation_t *sim = luaL_checkudata(L, 1, "Simulation");
    lua_pushinteger(L, (lua_Integer)(sim->height));
    return 1;
}

static const luaL_Reg simulation_methods[] =
{
    {"width", lua_simulation_get_width},
    {"height", lua_simulation_get_height},
    {NULL, NULL}
};

static void lua_simulation_register_metatable(lua_State *L)
{
    luaL_newmetatable(L, "Simulation");
    lua_pushvalue(L, -1);
    lua_setfield(L, -2, "__index");
    luaL_setfuncs(L, simulation_methods, 0);
    lua_pop(L, 1);
}

static void lua_simulation_push(lua_State *L, simulation_t *sim, const char *name)
{
    simulation_t **userdata = lua_newuserdata(L, sizeof(simulation_t *));
    *userdata = sim;
    luaL_getmetatable(L, "Simulation");
    lua_setmetatable(L, -2);
    if (name != NULL)
    {
        lua_setglobal(L, name);
    }
}

static int lua_simulation_get_name(lua_State *L)
{

    simulation_t **sim_ptr = luaL_checkudata(L, 1, "Simulation");

    if (sim_ptr == NULL || *sim_ptr == NULL) {
        return luaL_error(L, "Expected Simulation object");
    }

    simulation_t *sim = *sim_ptr;

    int x = lua_tointeger(L, 2);
    int y = lua_tointeger(L, 3);

    lua_Integer target;

    simulation_get_target(sim, x, y, &target);

    char *name = NULL;

    if (target < sim->particle_data_count)
    {

        name = sim->particle_data[target]->name;

    }

    lua_pushstring(L, name);
    return 1;
}

static int lua_simulation_set_name(lua_State *L)
{
    simulation_t **sim_ptr = luaL_checkudata(L, 1, "Simulation");

    if (sim_ptr == NULL || *sim_ptr == NULL) {
        return luaL_error(L, "Expected Simulation object");
    }

    simulation_t *sim = *sim_ptr;

    int x = lua_tointeger(L, 2);
    int y = lua_tointeger(L, 3);

    const char *name = luaL_checkstring(L, 4);

    lua_Integer target = -1;

    for (lua_Integer i = 0; i < sim->particle_data_count; ++i)
    {
        if (strcmp(sim->particle_data[i]->name, name) == 0)
        {
            target = i;
            break;
        }
    }

    if (target == -1)
    {

        fprintf(stderr, "voce sabe\n");

    }

    lua_pushboolean(L, simulation_set_target(sim, x, y, target));

    return 1;
}


static int lua_simulation_set_update(lua_State *L)
{
    simulation_t **sim_ptr = luaL_checkudata(L, 1, "Simulation");

    if (sim_ptr == NULL || *sim_ptr == NULL) {
        return luaL_error(L, "Expected Simulation object");
    }

    simulation_t *sim = *sim_ptr;

    int x = lua_tointeger(L, 2);
    int y = lua_tointeger(L, 3);

    lua_pushboolean(L, simulation_set_target_update(sim, x, y));

    return 1;
}

static int lua_simulation_register_particle(lua_State *L)
{

    simulation_t **sim_ptr = luaL_checkudata(L, 1, "Simulation");

    if (sim_ptr == NULL || *sim_ptr == NULL)
    {
        return luaL_error(L, "Expected Simulation object");
    }

    simulation_t *sim = *sim_ptr;

    if (!lua_istable(L, 2)) {
        return luaL_error(L, "Expected table as second argument");
    }

    lua_getfield(L, 2, "name");
    const char *name = lua_tostring(L, -1);
    lua_pop(L, 1);

    lua_getfield(L, 2, "r");
    int r = (int)luaL_checkinteger(L, -1);
    lua_pop(L, 1);

    lua_getfield(L, 2, "g");
    int g = (int)luaL_checkinteger(L, -1);
    lua_pop(L, 1);

    lua_getfield(L, 2, "b");
    int b = (int)luaL_checkinteger(L, -1);
    lua_pop(L, 1);

    lua_getfield(L, 2, "a");
    int a = (int)luaL_checkinteger(L, -1);
    lua_pop(L, 1);
    
    int lua_ref = LUA_NOREF;

    if (lua_getfield(L, 2, "update"))
    {

        lua_ref = luaL_ref(L, LUA_REGISTRYINDEX);

    }

    simulation_particle_data_t *p = new_simulation_particle_data(
        strdup(name), (SDL_Color){ r, g, b, a }, lua_ref
    );

    printf(
        "%d\n", 
        buffer_append(
        sim->particle_data_buffer,
        sizeof(simulation_particle_data_t *),
        (char *)&p
    ));

    sim->particle_data = (simulation_particle_data_t **)sim->particle_data_buffer->items;

    sim->particle_data[sim->particle_data_count++] = p;

    lua_pushboolean(L, 1);
    return 1;
}


simulation_particle_data_t *new_simulation_particle_data(char *name, SDL_Color color, int lua_ref)
{
    simulation_particle_data_t *data = malloc(sizeof(simulation_particle_data_t));
    if (!data)
    {
        return NULL;
    }
    data->name = name;
    data->color = color;
    data->lua_ref = lua_ref;
    return data;
}

void free_simulation_particle_data(simulation_particle_data_t *particle_data)
{

    if (particle_data == NULL)
    {

        return;

    }

    /*TODO: .*/

    free(particle_data);

}


simulation_t *new_simulation(lua_Unsigned width, lua_Unsigned height, lua_State *L)
{

    simulation_t *simulation = (simulation_t *)malloc(sizeof(simulation_t));

    if (simulation == NULL)
    {

        return NULL;

    }

    simulation->width = width;

    simulation->height = height;

    simulation->targets = (lua_Integer *)calloc(sizeof(lua_Integer) * simulation->width * simulation->height, 1);

    simulation->targets_update = (bool *)calloc(sizeof(bool) * simulation->width * simulation->height, 1);

    simulation->particle_data_buffer = new_buffer(sizeof(simulation_particle_data_t *));

    simulation->particle_data_count = 0;

    simulation->particle_data = (simulation_particle_data_t **)simulation->particle_data_buffer->items;

    simulation->lua_state = L;

    lua_simulation_register_metatable(simulation->lua_state);

    lua_simulation_push(simulation->lua_state, simulation, "simulation");

    lua_register(simulation->lua_state, "registerParticle", lua_simulation_register_particle);
    lua_register(simulation->lua_state, "getName", lua_simulation_get_name);
    lua_register(simulation->lua_state, "setName", lua_simulation_set_name);
    lua_register(simulation->lua_state, "setUpdate", lua_simulation_set_update);

    return simulation;

}

static void call_lua_update(lua_State *L, int lua_ref, int x, int y)
{
    if (lua_ref == LUA_NOREF)
    {
        return;
    }
    lua_rawgeti(L, LUA_REGISTRYINDEX, lua_ref);
    if (!lua_isfunction(L, -1))
    {
        lua_pop(L, 1);
        assert(0);
        return;
    }
    lua_pushinteger(L, x);
    lua_pushinteger(L, y);
    luascript_error_message(
        L, 
        lua_pcall(L, 2, 0, 0),
        false
    );
}


int simulation_add_particle_data(simulation_t *simulation, const char *name, SDL_Color color, int lua_ref)
{
   
    simulation_particle_data_t *p = new_simulation_particle_data((char *)name, color, lua_ref);
   
    buffer_append(simulation->particle_data_buffer, sizeof(simulation_particle_data_t *), (char *)&p);
    
    simulation->particle_data_count++;
 
    return 0;

}

int simulation_get_target(simulation_t *simulation, lua_Unsigned x, lua_Unsigned y, lua_Integer *target)
{
    
    if (x >= simulation->width || y >= simulation->height)
    {
        return 0;   
    }

    *target = simulation->targets[y * simulation->width + x];

    return 0;

}

int simulation_set_target(simulation_t *simulation, lua_Unsigned x, lua_Unsigned y, lua_Integer target)
{

    if (x >= simulation->width || y >= simulation->height)
    {
        return 0;   
    }

    simulation->targets[y * simulation->width + x] = target;

    return 1;

}

int simulation_set_target_update(simulation_t *simulation, lua_Unsigned x, lua_Unsigned y)
{

    if (x >= simulation->width || y >= simulation->height)
    {
        return 0;   
    }

    simulation->targets_update[y * simulation->width + x] = true;

    return 1;

}

int simulation_update(simulation_t *simulation)
{

    for (lua_Integer y = (lua_Integer)simulation->height - 1; 0 <= y; --y)
    {

        for (lua_Unsigned x = 0; x < simulation->width; ++x)
        {

            simulation->targets_update[y * simulation->width + x] = false;

        }
    
    }

    for (lua_Integer y = (lua_Integer)simulation->height - 1; 0 <= y; --y)
    {

        for (lua_Unsigned x = 0; x < simulation->width; ++x)
        {

            if (!simulation->targets_update[y * simulation->width + x])
            {

                lua_Integer target;

                simulation_get_target(simulation, x, y, &target);

                lua_Unsigned abs_target = (lua_Unsigned)llabs(target);

                if ((size_t)abs_target >= simulation->particle_data_count)
                {

                    continue;

                }

                call_lua_update(simulation->lua_state, simulation->particle_data[abs_target]->lua_ref, x, y);


            }

        }
    
    }

    return 0;

}

int simulation_render(simulation_t *simulation, SDL_Renderer *renderer)
{

    for (lua_Unsigned y = 0; y < simulation->height; ++y)
    {

        for (lua_Unsigned x = 0; x < simulation->width; ++x)
        {

            lua_Integer target;

            simulation_get_target(simulation, x, y, &target);

            lua_Unsigned abs_target = (lua_Unsigned)llabs(target);

            if ((size_t)abs_target >= simulation->particle_data_count)
            {

                continue;

            }

            SDL_Color color = simulation->particle_data[abs_target]->color;

            SDL_SetRenderDrawColor(renderer, color.r,  color.g,  color.b,  color.a);

            SDL_RenderDrawPoint(renderer, x, y);

        }

    }

    return 0;

}

void free_simulation(simulation_t *simulation)
{

    if (simulation == NULL)
    {

        return;

    }

    if (simulation->targets != NULL)
    {

        free(simulation->targets);

    }

    /*TODO: .*/

    free(simulation);

}
