#include "codexion.h"

static t_dongle *create_dondler(t_info_simulation	*info_simulation, int dongle_id)
{
	(void) info_simulation;
	t_dongle *dongle;

	dongle = malloc(sizeof(t_dongle));
	if (!dongle)
		return (NULL);
	memset(dongle, 0, sizeof(t_dongle));
	pthread_mutex_init(&dongle->lock, NULL);
	pthread_cond_init(&dongle->cond, NULL);
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
	if (coder_id == infos.number_of_coders)
		coder->right_dongle = infos.dongles[0];
	else
		coder->right_dongle = infos.dongles[coder->code_id];

	coder->left_dongle = infos.dongles[coder->code_id - 1];
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
	return (0);
}

t_coder	*init_list_of_coders(t_info_simulation info)
{
	t_coder *list_of_coder;
	t_coder *tmp;
	int verify_error;
	int i;

	i = 1;
	list_of_coder = create_coder(info, i++);
	if (!list_of_coder)
		return (clear_allocation(&info));
	while (i <= info.number_of_coders)
	{
		verify_error = lst_append_new_coders(list_of_coder, info, i++);
		if (verify_error)
			return (clear_allocation(&info));
	}
	tmp = list_of_coder;
	while (tmp->code_id < info.number_of_coders)
		tmp = tmp->right_coder;
	//circular list:
	tmp->right_coder = list_of_coder;
	return (list_of_coder);
}

void *init_info_simulation(t_info_simulation	*infos, char	**av)
{
	int i;
	struct timespec time;

    infos->number_of_coders = atoi(av[1]);
    infos->time_to_burnout = atoi(av[2]);
    infos->time_to_compile = atoi(av[3]);
    infos->time_to_debug = atoi(av[4]);
    infos->time_to_refactor = atoi(av[5]);
    infos->number_of_compiles_required = atoi(av[6]);
    infos->dongle_cooldown = atoi(av[7]);
    if (strcmp(av[8], "fifo") == 0)
        infos->scheduler = 'F';
    else
		infos->scheduler = 'E';
	infos->running = 1;

	clock_gettime(CLOCK_MONOTONIC, &time);
	infos->start_ms = ((time.tv_sec * 1000) + (time.tv_nsec / 1000000));
		

	infos->dongles = malloc(infos->number_of_coders * sizeof(t_dongle *));
	if (!infos->dongles)
		return (NULL);
	i = 0;
	while(i < infos->number_of_coders)
	{
		infos->dongles[i] = create_dondler(infos, i);
		if (!infos->dongles[i++])
			return (clear_allocation(infos));
	}

    infos->supervisor = malloc(sizeof(t_supervisor));
	infos->supervisor->info = infos;
	pthread_mutex_init(&infos->lock, NULL);

    infos->printl = malloc(sizeof(t_print_locked));
    memset(infos->printl, 0, sizeof(t_print_locked));
    pthread_mutex_init(&infos->printl->lock, NULL);
    infos->printl->print = printf;

	return ("Everything OK!");
}
