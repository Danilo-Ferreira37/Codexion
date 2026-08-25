/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   routine_utils.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dosorio- <dosorio-@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/24 21:54:27 by dosorio-          #+#    #+#             */
/*   Updated: 2026/08/25 18:43:27 by dosorio-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

void	*wakeup_all_threads(t_dongle **dongles)
{
	int	i;

	i = -1;
	while (dongles[++i])
	{
		pthread_mutex_lock(&dongles[i]->lock);
		pthread_cond_broadcast(&dongles[i]->cond);
		pthread_mutex_unlock(&dongles[i]->lock);
	}
	return (NULL);
}

void	join_threads(t_info_simulation *info)
{
	t_coder	*tmp;

	pthread_join(info->supervisor->thread, NULL);
	tmp = info->list_of_coders;
	while (1)
	{
		pthread_join(tmp->thread, NULL);
		if (tmp->coder_id == info->number_of_coders)
			break ;
		tmp = tmp->right_coder;
	}
}

void	get_dongle(int coder_id, t_dongle *dongle, t_info_simulation *info)
{
	dongle->owner = coder_id;
	printl("%d %d has taken a dongle\n", info, coder_id);
}

int	verify_1_coder(t_coder *self, t_info_simulation *info)
{
	if (info->number_of_coders == 1)
	{
		pthread_mutex_lock(&self->right_dongle->lock);
		self->right_dongle->owner = self->coder_id;
		printf("%d %d has taken a dongle\n", current_milliseconds(info),
			self->coder_id);
		pthread_mutex_unlock(&self->right_dongle->lock);
		return (1);
	}
	return (0);
}
