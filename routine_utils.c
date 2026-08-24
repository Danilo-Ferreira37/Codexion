/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   routine_utils.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dosorio- <dosorio-@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/24 21:54:27 by dosorio-          #+#    #+#             */
/*   Updated: 2026/08/24 21:55:13 by dosorio-         ###   ########.fr       */
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
