#include "codexion.h"

int main(int ac, char **av)
{
    t_info_simulation info_simulation;
    t_coder  *list_of_coders;

    if (ac != 9)
        return (error("Invalid number of arguments!"));
    if (args_parse(av))
        return (-1);

    init_info_simulation(&info_simulation, av);
    list_of_coders = init_list_of_coders(info_simulation);
    if (!list_of_coders)
        return (error("Memory allocation failure!"));
    

    return (0);
}