/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   actions_utils.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dosorio- <dosorio-@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/24 15:53:20 by dosorio-          #+#    #+#             */
/*   Updated: 2026/08/24 21:03:42 by dosorio-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

int	debug_and_refactor(t_coder *self, t_info_simulation *info)
{
	if (stopped(info))
		return (-1);
	printl("%d %d is debugging\n", info, self->coder_id);
	times_wait(info->time_to_debug);
	if (stopped(info))
		return (-1);
	printl("%d %d is refactoring\n", info, self->coder_id);
	times_wait(info->time_to_refactor);
	return (0);
}

void	printl(char *message, t_info_simulation *info, int coder_id)
{
	pthread_mutex_lock(&info->lock);
	if (!info->running)
	{
		pthread_mutex_unlock(&info->lock);
		return ;
	}
	pthread_mutex_lock(&info->printl->lock);
	info->printl->print(message, current_milliseconds(info), coder_id);
	pthread_mutex_unlock(&info->printl->lock);
	pthread_mutex_unlock(&info->lock);
}

void	append_queue(t_coder **queue, t_coder *coder)
{
	if (queue[0] == coder || queue[1] == coder)
		return ;
	if (queue[0])
		queue[1] = coder;
	else
		queue[0] = coder;
}

int	lst_append_new_coders(t_coder *coder, t_info_simulation infos,
		int id_coder)
{
	t_coder	*next_coder;
	t_coder	*tmp;

	tmp = coder;
	while (tmp->right_coder)
		tmp = tmp->right_coder;
	next_coder = create_coder(infos, id_coder);
	if (!next_coder)
		return (error(NULL, NULL));
	tmp->right_coder = next_coder;
	return (0);
}

int	stopped(t_info_simulation *info)
{
	pthread_mutex_lock(&info->lock);
	if (!info->running)
	{
		pthread_mutex_unlock(&info->lock);
		return (-1);
	}
	pthread_mutex_unlock(&info->lock);
	return (0);
}
