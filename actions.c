#include "codexion.h"
#include <time.h>

void time_wait(int time_ms)
{
    usleep(time_ms * 1000);
}

int current_milliseconds(t_info_simulation infos)
{
    struct timespec time;
    int current_ms;

    clock_gettime(CLOCK_MONOTONIC, &time);
    current_ms = ((time.tv_sec * 1000) + (time.tv_nsec / 1000000));
    return (current_ms - infos.start_ms);
}

void compile(t_coder *coder, t_info_simulation infos)
{
    int left_d_is_locked;
    int right_d_is_locked;

    left_d_is_locked = pthread_mutex_trylock(&coder->left_dongle->lock);
    right_d_is_locked = pthread_mutex_trylock(&coder->right_dongle->lock);
    if (!left_d_is_locked)
    {
        printf("%d %d has taken a dongle\n", current_milliseconds(infos), coder->code_id);
        coder->has_l_dongle = 1;
    }
    if (!right_d_is_locked)
    {
        printf("%d %d has taken a dongle\n", current_milliseconds(infos), coder->code_id);
        coder->has_r_dongle = 1;
    }
    if ((left_d_is_locked || !coder->has_l_dongle) || (right_d_is_locked || !coder->has_r_dongle))
        return;

    printf("%d %d is compiling\n", current_milliseconds(infos), coder->code_id);
    time_wait(infos.time_to_copile);

    coder->total_copiles++;
    coder->time_without_copile = 0;
    
    pthread_cond_timedwait(&coder->left_dongle->lock, infos.dongle_cooldown);
    pthread_cond_timedwait(&coder->right_dongle->lock, infos.dongle_cooldown);
}

void debug(t_coder *coder, t_info_simulation infos)
{

}