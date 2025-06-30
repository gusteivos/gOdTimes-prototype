
#ifndef LOOP_H
#define LOOP_H

#include <stddef.h>
#include <stdlib.h>
#include <errno.h>

#include "buffer.h"

#include <SDL2/SDL.h>



#ifndef LOOP_MILLISECONDS_PER_SECOND
#define LOOP_MILLISECONDS_PER_SECOND 1000
#endif

#define loop_set_target_elapsed_performance_counter_ffps(fps) \
loop_set_target_elapsed_performance_counter_fms(              \
    (float)LOOP_MILLISECONDS_PER_SECOND / (float)fps          \
    )

#define loop_set_target_elapsed_performance_counter_fs(s)     \
loop_set_target_elapsed_performance_counter_fms(              \
    (float)LOOP_MILLISECONDS_PER_SECOND * (float)s            \
    )

#ifndef LOOP_CALLBACK_BUFFER_SIZE
#define LOOP_CALLBACK_BUFFER_SIZE sizeof(void *) * sizeof(void *)
#endif


typedef void (* loop_callback_t)(void);


extern Uint64 loop_performance_frequency;
extern Uint64 loop_last_performance_counter;
extern float  loop_fdelta_time;
extern Uint64 loop_u64delta_time;
extern Uint64 loop_elapsed_performance_counter;
extern Uint64 loop_target_elapsed_performance_counter;

extern buffer_t *loop_start_callback_buffer;
extern buffer_t *loop_on_start_callback_buffer;
extern buffer_t *loop_on_middle_callback_buffer;
extern buffer_t *loop_on_end_callback_buffer;
extern buffer_t *loop_end_callback_buffer;

extern Uint64 _loop_current_performance_counter;
extern Uint64 _loop_performance_counter_per_millisecond;


int  loop_init     (void);
int  loop_callback_buffer_append(buffer_t *buffer, loop_callback_t callback);
int  loop_callback_buffer_remove(buffer_t *buffer, loop_callback_t callback);
int  loop_start    (void);
int  loop_set_target_elapsed_performance_counter_u64ms(Uint64 ms);
int  loop_set_target_elapsed_performance_counter_fms(float ms);
int  loop_on_start (void);
int  loop_on_middle(void);
int  loop_on_end   (void);
int  loop_end      (void);
void loop_quit     (void);

#endif/*LOOP_H*/
