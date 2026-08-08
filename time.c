#include "codexion.h"

void time_wait(int time_ms)
{
    usleep(time_ms * 1000);
}

int current_milliseconds(t_info_simulation *infos)
{
    struct timespec time;
    int current_ms;

    clock_gettime(CLOCK_MONOTONIC, &time);
    current_ms = ((time.tv_sec * 1000) + (time.tv_nsec / 1000000));
    //printf("\ntempo do comeco %d   tempo atual %d\n", infos->start_ms, current_ms);
    return (current_ms - infos->start_ms);
}

struct timespec get_cooldwn(t_info_simulation infos)
{
    struct timespec time;

    clock_gettime(CLOCK_REALTIME, &time);
    //printf("%ld, %ld", time.tv_sec, time.tv_nsec);
    time.tv_sec += infos.dongle_cooldown / 1000;
    time.tv_nsec += (infos.dongle_cooldown % 1000) * 1000000;
    if (time.tv_nsec > 1000000000)
    {
        time.tv_sec += 1;
        time.tv_nsec -= 1000000000;
    }
    return (time);
}
