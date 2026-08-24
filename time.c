/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   time.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dosorio- <dosorio-@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/24 15:52:48 by dosorio-          #+#    #+#             */
/*   Updated: 2026/08/24 21:04:14 by dosorio-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

void	times_wait(int time_ms)
{
	usleep(time_ms * 1000);
}

int	current_milliseconds(t_info_simulation *infos)
{
	struct timespec	time;
	int				current_ms;

	clock_gettime(CLOCK_MONOTONIC, &time);
	current_ms = ((time.tv_sec * 1000) + (time.tv_nsec / 1000000));
	return (current_ms - infos->start_ms);
}

struct timespec	get_cooldwn(t_info_simulation *infos)
{
	struct timespec	time;

	clock_gettime(CLOCK_REALTIME, &time);
	time.tv_sec += infos->dongle_cooldown / 1000;
	time.tv_nsec += (infos->dongle_cooldown % 1000) * 1000000;
	if (time.tv_nsec > 1000000000)
	{
		time.tv_sec += 1;
		time.tv_nsec -= 1000000000;
	}
	return (time);
}
