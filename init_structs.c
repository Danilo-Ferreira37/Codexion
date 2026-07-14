#include "codexion.h"

void    init_info_simulation(t_info_simulation *info_simulation, char **av)
{
    info_simulation->number_of_coders = atoi(av[1]);
    info_simulation->time_to_burnout = atoi(av[2]);
    info_simulation->time_to_copile = atoi(av[3]);
    info_simulation->time_to_debug = atoi(av[4]);
    info_simulation->time_to_refactor = atoi(av[5]);
    info_simulation->number_of_compiles_required = atoi(av[6]);
    info_simulation->dongle_cooldown = atoi(av[7]);
    if (!strcmp(av[8], "fifo"))
        info_simulation->scheduler = 'f';
    else
        info_simulation->scheduler = 'e';
}

