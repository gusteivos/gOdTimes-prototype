#include "loop.h"


Uint64 loop_performance_frequency;
Uint64 loop_last_performance_counter;
float  loop_fdelta_time;
Uint64 loop_u64delta_time;
Uint64 loop_elapsed_performance_counter;
Uint64 loop_target_elapsed_performance_counter;

buffer_t *loop_start_callback_buffer;
buffer_t *loop_on_start_callback_buffer;
buffer_t *loop_on_middle_callback_buffer;
buffer_t *loop_on_end_callback_buffer;
buffer_t *loop_end_callback_buffer;

Uint64 _loop_current_performance_counter;
Uint64 _loop_performance_counter_per_millisecond;


static void _loop_run_callback_buffer(buffer_t *buffer)
{

    size_t count = buffer->count / sizeof(void *);

    for (size_t i = 0; i < count; ++i)
    {

        loop_callback_t callback = *(loop_callback_t *)(buffer->items + i * sizeof(void *));

        if (callback)
        {

            callback();

        }

    }

}


int loop_init(void)
{

    loop_start_callback_buffer     = new_buffer(LOOP_CALLBACK_BUFFER_SIZE);

    if (!loop_start_callback_buffer)
    {

        loop_quit();

        return EPERM;

    }

    loop_on_start_callback_buffer  = new_buffer(LOOP_CALLBACK_BUFFER_SIZE);

    if (!loop_on_start_callback_buffer)
    {

        loop_quit();

        return ENOENT;

    }

    loop_on_middle_callback_buffer = new_buffer(LOOP_CALLBACK_BUFFER_SIZE);

    if (!loop_on_middle_callback_buffer)
    {

        loop_quit();

        return ESRCH;

    }

    loop_on_end_callback_buffer    = new_buffer(LOOP_CALLBACK_BUFFER_SIZE);

    if (!loop_on_end_callback_buffer)
    {

        loop_quit();

        return EINTR;

    }

    loop_end_callback_buffer       = new_buffer(LOOP_CALLBACK_BUFFER_SIZE);

    if (!loop_end_callback_buffer)
    {

        loop_quit();

        return EIO;

    }

    return 0;
}

int loop_callback_buffer_append(buffer_t *buffer, loop_callback_t callback)
{

    return buffer_append(buffer, sizeof(loop_callback_t), (char *)&callback);

}

int loop_callback_buffer_remove(buffer_t *buffer, loop_callback_t callback)
{

    /*TODO: .*/

    return 0;

}

int  loop_start    (void)
{

    loop_performance_frequency = SDL_GetPerformanceFrequency();

    loop_last_performance_counter = SDL_GetPerformanceCounter();

    loop_elapsed_performance_counter = 0;


    _loop_performance_counter_per_millisecond = loop_performance_frequency / LOOP_MILLISECONDS_PER_SECOND;


    if (loop_start_callback_buffer == NULL)
    {

        return EFAULT;

    }

    _loop_run_callback_buffer(loop_start_callback_buffer);

    return 0;

}

int loop_set_target_elapsed_performance_counter_u64ms(Uint64 ms)
{

    if (loop_performance_frequency == 0)
    {

        return EPERM;

    }

    loop_target_elapsed_performance_counter = _loop_performance_counter_per_millisecond * ms;

    return 0;

}

int loop_set_target_elapsed_performance_counter_fms(float ms)
{

    if (loop_performance_frequency == 0)
    {

        return EPERM;

    }

    loop_target_elapsed_performance_counter = (Uint64)(_loop_performance_counter_per_millisecond * ms);

    return 0;

}

int  loop_on_start (void)
{

    if (loop_performance_frequency == 0)
    {

        return EPERM;

    }

    int return_value = 0;

    if (loop_on_start_callback_buffer != NULL)
    {

        _loop_run_callback_buffer(loop_on_start_callback_buffer);

    }
    else
    {

        return_value = EFAULT;

    }

    _loop_current_performance_counter = SDL_GetPerformanceCounter();

    loop_fdelta_time = (float)(_loop_current_performance_counter - loop_last_performance_counter) / (float)loop_performance_frequency;

    loop_u64delta_time = (_loop_current_performance_counter - loop_last_performance_counter) / _loop_performance_counter_per_millisecond;

    loop_last_performance_counter = _loop_current_performance_counter;

    return return_value;

}

int  loop_on_middle(void)
{

    if (loop_performance_frequency == 0)
    {

        return EPERM;

    }

    if (loop_on_middle_callback_buffer == NULL)
    {

        return EFAULT;

    }

    _loop_run_callback_buffer(loop_on_middle_callback_buffer);

    return 0;

}

int  loop_on_end   (void)
{

    if (loop_performance_frequency == 0)
    {

        return EPERM;

    }

    int return_value = 0;

    if (loop_on_end_callback_buffer != NULL)
    {

        _loop_run_callback_buffer(loop_on_end_callback_buffer);

    }
    else
    {

        return_value = EFAULT;

    }

    _loop_current_performance_counter = SDL_GetPerformanceCounter();

    loop_elapsed_performance_counter = _loop_current_performance_counter - loop_last_performance_counter;

    if (loop_elapsed_performance_counter < loop_target_elapsed_performance_counter)
    {

        Uint64 remainin_elapsed_performance_counter = loop_target_elapsed_performance_counter - loop_elapsed_performance_counter;

        Uint64 milliseconds = remainin_elapsed_performance_counter / _loop_performance_counter_per_millisecond;

        if (milliseconds > 0)
        {

            SDL_Delay((Uint32)milliseconds);

        }

#ifdef LOOP_C_SPIN_WAIT
        while (
            (SDL_GetPerformanceCounter() - loop_last_performance_counter) < loop_target_elapsed_performance_counter
            )
        {
            ;
        }
#endif

    }

    return return_value;

}

int  loop_end      (void)
{

    if (loop_performance_frequency == 0)
    {

        return EPERM;

    }

    if (loop_end_callback_buffer == NULL)
    {

        return EFAULT;

    }

    _loop_run_callback_buffer(loop_end_callback_buffer);

    return 0;

}

void loop_quit     (void)
{

    /* TODO: . */

}
