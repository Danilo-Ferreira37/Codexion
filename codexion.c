#include "codexion.h"

int main(int ac, char **av)
{
    t_info_simulation info_simulation;
    if (ac != 9)
        return (error("Invalid number of arguments!"));
    if (args_parse(av))
        return (-1);
    init_info_simulation(&info_simulation, av);

    return (0);
}