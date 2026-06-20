#include "codexion.h"


int main(int ac, char **av)
{
    t_node info_list;

    if (ac != 9)
    {
        error("Invalid number of arguments!");
        return (1);
    }
    if (args_parse(av))
        return (1);

    if (get_init_info(av, &info_list))
        return (1);

    return (0);
}