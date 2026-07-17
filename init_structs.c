#include "codexion.h"

static t_dongle *create_dondler(t_info_simulation	*info_simulation, int dongle_id)
{
	t_dongle *dongle;

	dongle = malloc(sizeof(t_dongle));
	if (!dongle)
		return (NULL);
	memset(dongle, 0, sizeof(t_dongle));
	dongle->cooldwn = info_simulation->dongle_cooldown;
	dongle->dongle_id = dongle_id;
	return (dongle);
}

static	t_coder *create_coder(t_info_simulation infos, int coder_id)
{
	t_coder *coder;

	coder = malloc(sizeof(t_coder));
	if (!coder)
		return (NULL);
	memset(coder, 0, sizeof(t_coder));
	coder->code_id = coder_id;
	coder->time_to_die = infos.time_to_burnout;
	if (coder_id == infos.number_of_coders - 1)
		coder->right_dongle = infos.dongles[0];
	else
		coder->right_dongle = infos.dongles[coder->code_id + 1];

	coder->left_dongle = infos.dongles[coder->code_id];
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
		return (clear_allocation(NULL, info.dongles));
	while (i < info.number_of_coders)
	{
		verify_error = lst_append_new_coders(list_of_coder, info, i++);
		if (verify_error)
			return (clear_allocation(list_of_coder, info.dongles));
	}
	tmp = list_of_coder;
	while (tmp->right_coder)
		tmp = tmp->right_coder;
	tmp->right_coder = list_of_coder;
	list_of_coder->left_coder = tmp;
	return (list_of_coder);
}

void *init_info_simulation(t_info_simulation	*info_simulation, char	**av)
{
	int i;

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

	info_simulation->dongles = malloc(info_simulation->number_of_coders * sizeof(t_dongle *));
	if (!info_simulation->dongles)
		return (NULL);
	i = 0;
	while(i < info_simulation->number_of_coders)
	{
		info_simulation->dongles[i] = create_dondler(info_simulation, i);
		if (!info_simulation->dongles[i++])
			return (clear_allocation(NULL, info_simulation->dongles));
	}
	return ("Everything OK!");
}
