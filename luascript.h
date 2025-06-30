
#ifndef LUASCRIPT_H
#define LUASCRIPT_H

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <assert.h>

#include <lua5.3/lua.h>
#include <lua5.3/lauxlib.h>

#include "buffer.h"


struct luascript_pair_descriptor_s
{

    bool field;

    int type;

    bool array_table_number_integer;

    char *name;

    struct luascript_pair_descriptor_s *pairs;

};

typedef struct luascript_pair_descriptor_s luascript_pair_descriptor_t;


#define luascript_do_file(L, filename, thread_status) luascript_do_filex(L, filename, NULL, thread_status)

#define luascript_error_message(L, status, can_exit) luascript_error_messagex(L, status, can_exit, __FILE__, __LINE__)


int luascript_do_filex(lua_State *L, const char *filename, const char *mode, int *thread_status);

int luascript_error_messagex(lua_State *L, int status, bool can_exit, const char *file, int line);

int luascript_serialize_pairs(lua_State *L, luascript_pair_descriptor_t *pairs, buffer_t *buffer, size_t *error_count, buffer_t *error_buffer);

int luascript_deserialize_pairs(lua_State *L, luascript_pair_descriptor_t *pairs, buffer_t *buffer);

#endif/*LUASCRIPT_H*/
