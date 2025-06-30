
#ifndef BUFFER_H
#define BUFFER_H

#include <stddef.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>


struct buffer_s
{

    char *items;

    size_t capacity;

    size_t count;

};

typedef struct buffer_s buffer_t;


buffer_t *new_buffer(size_t capacity);

int buffer_append(
    buffer_t *buffer,
    size_t count,
    char *items
    );

int buffer_truncate(
    buffer_t *buffer,
    size_t count,
    char *items
    );

int buffer_push(
    buffer_t *buffer,
    char item
    );

int buffer_pop(
    buffer_t *buffer,
    char *item
    );

int buffer_peek(
    buffer_t *buffer,
    char *item
    );

void free_buffer(buffer_t *buffer);

#endif/*BUFFER_H*/