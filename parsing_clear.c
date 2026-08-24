/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing_clear.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dosorio- <dosorio-@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/24 15:52:58 by dosorio-          #+#    #+#             */
/*   Updated: 2026/08/24 21:03:59 by dosorio-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

int	error(char *error_msg, t_info_simulation *infos)
{
	if (infos)
		clear_allocation(infos);
	if (!error_msg)
		return (-1);
	fprintf(stderr, "Error: %s\n", error_msg);
	return (-1);
}

static void	clear_allocation_utils(t_info_simulation *infos)
{
	int	i;

	i = 0;
	if (infos->dongles)
	{
		while (infos->dongles[i])
		{
			pthread_cond_destroy(&infos->dongles[i]->cond);
			pthread_mutex_destroy(&infos->dongles[i]->lock);
			free(infos->dongles[i++]);
		}
		free(infos->dongles);
	}
	if (infos->printl)
	{
		pthread_mutex_destroy(&infos->printl->lock);
		free(infos->printl);
	}
	if (infos->supervisor)
		free(infos->supervisor);
	pthread_mutex_destroy(&infos->lock);
}

void	*clear_allocation(t_info_simulation *infos)
{
	t_coder	*tmp;
	t_coder	*next;

	if (!infos)
		return (NULL);
	if (infos->list_of_coders)
	{
		tmp = infos->list_of_coders;
		while (tmp->right_coder != infos->list_of_coders)
			tmp = tmp->right_coder;
		tmp->right_coder = NULL;
		tmp = infos->list_of_coders;
		while (tmp)
		{
			next = tmp->right_coder;
			free(tmp);
			tmp = next;
		}
	}
	clear_allocation_utils(infos);
	return (NULL);
}

static int	error_in_num(char *arg)
{
	int	i;

	i = 0;
	if (arg[i] == '+' || arg[i] == '-')
		i++;
	if (!arg[i])
		return (error("The program received a invalid number", NULL));
	while (arg[i])
	{
		if (arg[i] < '0' || arg[i] > '9')
			return (error("The program received a invalid number", NULL));
		i++;
	}
	return (0);
}

int	args_parse(char **av)
{
	int	i;

	i = 1;
	while (i < 8)
	{
		if (error_in_num(av[i]))
			return (-1);
		if (atoi(av[i]) <= 0)
			return (error("The numerical arguments must be positive integers!",
					NULL));
		i++;
	}
	if (strcmp(av[i], "fifo") && strcmp(av[i], "edf"))
		return (
			error
			("The program received a invalid scheduling! "
				"(Must be exactly 'fifo' or 'edf')",
				NULL));
	return (0);
}
