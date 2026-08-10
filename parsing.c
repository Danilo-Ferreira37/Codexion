#include "codexion.h"

int error(char *error_msg)
{
	if (!error_msg)
		return (-1);
    fprintf(stderr, "Error: %s\n", error_msg);
	return (-1);
}

void	*clear_allocation(t_info_simulation *infos)
{
	t_coder *tmp;
	int i;

	if (infos->list_of_coders)
	{
		while(infos->list_of_coders->code_id < infos->number_of_coders)
		{
			tmp = infos->list_of_coders;
			infos->list_of_coders = infos->list_of_coders->right_coder;
			free(tmp);
		}
	}
	if (infos->dongles)
	{
		i = 0;
		while(infos->dongles[i])
			free(infos->dongles[i++]);
		free(infos->dongles);
	}
	return (NULL);
}

static int error_in_num(char *arg)
{
	int i;

	i = 0;
	if (arg[i] == '+' || arg[i] == '-')
		i++;
	if (!arg[i])
		return (error("The program received a invalid number"));
	while (arg[i])
	{
		if (arg[i] < '0' || arg[i] > '9') 
			return (error("The program received a invalid number"));
		i++;
	}
	return (0);
}

int args_parse(char **av)
{
    int i;

    i = 1;
    while (i < 8)
    {
		if (error_in_num(av[i]))
			return (-1);
		if (atoi(av[i]) < 0)
			return(error("The numerical arguments must be positive integers!"));
		i++;
    }
	if (strcmp(av[i], "fifo") && strcmp(av[i], "edf"))
		return (error("The program received a invalid scheduling! (Must be exactly 'fifo' or 'edf')"));
	return (0);
}
