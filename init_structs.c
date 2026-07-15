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
    if (strcmp(av[8], "fifo") == 0)
        info_simulation->scheduler = 'F';
    else
        info_simulation->scheduler = 'E';
}

static	t_coder *create_coder(t_info_simulation infos, int id_coder)
{
	t_coder *coder;

	coder = malloc(sizeof(t_coder));
	if (!coder)
		return (NULL);
	memset(coder, 0, sizeof(t_coder));
	coder->code_id = id_coder;
	coder->time_to_die = infos.time_to_burnout;
	return (coder);
}

static int lst_append_new_coders(t_coder *coder, t_info_simulation infos, int id_coder)
{
	t_coder *next_coder;
	t_coder *tmp;

	tmp = coder;
	while(tmp->right_coder)
		tmp = tmp->right_coder;
	next_coder = create_coder(infos, id_coder);
	if (!next_coder)
		return (error(NULL));
	tmp->right_coder = next_coder;
	next_coder->left_coder = tmp;
	return (0);
}

t_coder	*init_list_of_coders(t_info_simulation info)
{
	t_coder *list_of_coder;
	t_coder *tmp;
	int verify_error;
	int i;

	i = 0;
	list_of_coder = create_coder(info, i++);
	if (!list_of_coder)
		return (NULL);
	while (i < info.number_of_coders)
	{
		verify_error = lst_append_new_coders(list_of_coder, info, i++);
		if (verify_error)
			return (clear_allocation(list_of_coder));
	}
	tmp = list_of_coder;
	while (tmp->right_coder)
		tmp = tmp->right_coder;
	tmp->right_coder = list_of_coder;
	list_of_coder->left_coder = tmp;
	return (list_of_coder);
}
