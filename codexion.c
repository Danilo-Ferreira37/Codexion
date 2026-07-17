#include "codexion.h"

int main(int ac, char **av)
{
    t_info_simulation info_simulation;
    t_coder  *list_of_coders;

    if (ac != 9)
        return (error("Invalid number of arguments!"));
    if (args_parse(av))
        return (-1);
    if (!init_info_simulation(&info_simulation, av))
        return (error("Memory allocation failure!"));
    list_of_coders = init_list_of_coders(info_simulation);
    if (!list_of_coders)
        return (error("Memory allocation failure!"));
/*     if (info_simulation.scheduler == 'F')
        fifo(list_of_coders, info_simulation);
    else
        edf(list_of_coders, info_simulation); */

    return (0);
}
