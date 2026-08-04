#include "codexion.h"

int main(int ac, char **av)
{
    t_info_simulation info_simulation;

    if (ac != 9)
        return (error("Invalid number of arguments!"));
    if (args_parse(av))
        return (-1);
    if (!init_info_simulation(&info_simulation, av))
        return (error("Memory allocation failure!"));
    info_simulation.list_of_coders = init_list_of_coders(info_simulation);
    if (!info_simulation.list_of_coders)
        return (error("Memory allocation failure!"));
    create_threads(&info_simulation);
    join_threads(&info_simulation);
    return (0);
}
