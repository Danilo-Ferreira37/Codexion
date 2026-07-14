#include "codexion.h"

void    init_info_simulation(t_info_simulation	*info_simulation, char	**av)
{
    info_simulation->number_of_coders = atoi(av[1]);
    info_simulation->time_to_burnout = atoi(av[2]);
    info_simulation->time_to_copile = atoi(av[3]);
    info_simulation->time_to_debug = atoi(av[4]);
    info_simulation->time_to_refactor = atoi(av[5]);
    info_simulation->number_of_compiles_required = atoi(av[6]);
    info_simulation->dongle_cooldown = atoi(av[7]);
    if (!strcmp(av[8], "fifo"))
        info_simulation->scheduler = 'F';
    else
        info_simulation->scheduler = 'E';
}

static	t_coder *create_coder(int id_coder, t_info_simulation infos)
{
	t_coder *coder;

	coder = malloc(sizeof(t_coder));
	if (!coder)
	{
		error("Memory allocaation failure!");
		return (NULL);
	}
	memset(coder, 0, sizeof(t_coder));
	coder->code_id = id_coder;
	coder->left_dongle = NULL;
	coder->right_dongle = NULL;
	coder->left_coder = NULL;
	coder->right_coder = NULL;

	coder->time_without_copile = 0;
	coder->total_copiles = 0;
	coder->time_to_die = infos.time_to_burnout;

	return (coder);
}

t_node	*init_list_of_coders(int number_of_coders, t_info_simulation infos)
{
	t_node *list_of_coder;
	int i;

	i = 0;
	list_of_coder = malloc(sizeof(t_coder) * number_of_coders);
	while (i < number_of_coders)
	{                   
		 
		.
	}
}