#include "luascript.h"


int luascript_do_filex(lua_State *L, const char *filename, const char *mode, int *thread_status)
{

    int luaL_loadfilex_return = luaL_loadfilex(L, filename, mode);

    if (luaL_loadfilex_return != LUA_OK)
    {

        return luaL_loadfilex_return;

    }

    int lua_pcall_return = lua_pcall(L, 0, LUA_MULTRET, 0);

    if (thread_status != NULL)
    {

        *thread_status = lua_pcall_return;

    }

    return LUA_OK;

}

int luascript_error_messagex(lua_State *L, int status, bool can_exit, const char *file, int line)
{

    if (status != LUA_OK && status != LUA_YIELD) /* or status > LUA_YIELD */
    {

        if (lua_isstring(L, -1) == 1)
        {

            const char *error_type = NULL;

            switch (status)
            {

            case LUA_ERRRUN:
                error_type = "Runtime Error";
                break;

            case LUA_ERRSYNTAX:
                error_type = "Syntax Error";
                break;

            case LUA_ERRMEM:
                error_type = "Memory Allocation Error";
                break;

            case LUA_ERRGCMM:
                error_type = "Garbage Collector Error";
                break;

            case LUA_ERRERR:
                error_type = "Error While Handling Error";
                break;

            case LUA_ERRFILE:
                error_type = "File Error";
                break;

            }

            const char *error_message = lua_tostring(L, -1);

            fprintf(
                stderr,
                "[lua] %s from FILE %s on LINE %d: \"%s\".\n",
                error_type,
                file,
                line,
                error_message
            );

        }

        if (can_exit)
        {

            exit(EXIT_FAILURE);

        }

        lua_pop(L, 1);

    }

    return status;

}

int luascript_serialize_pairs(lua_State *L, luascript_pair_descriptor_t *pairs, buffer_t *buffer, size_t *error_count, buffer_t *error_buffer)
{

    for (luascript_pair_descriptor_t *pair_descriptor = pairs; pair_descriptor->type != LUA_TNONE; ++pair_descriptor)
    {

        if (pair_descriptor->field)
        {

            lua_getfield(L, -1, pair_descriptor->name);

        }
        else
        {

            lua_getglobal(L, pair_descriptor->name);

        }

        if (lua_type(L, -1) != pair_descriptor->type)
        {

            lua_pop(L, 1);

            assert(0 && "TODO: update error_count and error_buffer.");

            continue;

        }

        switch (pair_descriptor->type)
        {

        case LUA_TBOOLEAN:
            {

                bool bool_ = lua_toboolean(L, -1) == 1 ? true : false;

                buffer_append(buffer, sizeof(bool), (char *)&bool_);

            }
            break;

        case LUA_TNUMBER:
            if (pair_descriptor->array_table_number_integer)
            {

                lua_Integer Integer = lua_tointeger(L, -1);
                
                buffer_append(buffer, sizeof(lua_Integer), (char *)&Integer);
                
            }
            else
            {

                lua_Number Number = lua_tonumber(L, -1);

                buffer_append(buffer, sizeof(lua_Number), (char *)&Number);

            }
            break;

        case LUA_TSTRING:
            {

                const char *lua_string = lua_tostring(L, -1);

                size_t lua_string_size = strlen(lua_string) + 1;

                char *string = malloc(lua_string_size);

                memcpy(string, lua_string, lua_string_size);

                buffer_append(buffer, sizeof(char *), (char *)&string);

            }
            break;

        case LUA_TTABLE:
            if (pair_descriptor->array_table_number_integer)
            {

                assert(0 && "TODO: .");

            }
            else
            {

                luascript_serialize_pairs(L, pair_descriptor->pairs, buffer, error_count, error_buffer);

            }
            break;

        }

        lua_pop(L, 1);

    }

    return LUA_OK;

}

int luascript_deserialize_pairs(lua_State *L, luascript_pair_descriptor_t *pairs, buffer_t *buffer)
{



}
