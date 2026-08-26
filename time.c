/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   time.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dosorio- <dosorio-@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/24 15:52:48 by dosorio-          #+#    #+#             */
/*   Updated: 2026/08/25 17:06:00 by dosorio-         ###   ########.fr       */
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

struct timespec	get_cooldown_deadline(t_dongle *dongle,
		t_info_simulation *infos)
{
	struct timespec	time;
	long			remaining_ms;

	clock_gettime(CLOCK_REALTIME, &time);
	remaining_ms = (dongle->released_ms + infos->dongle_cooldown)
		- current_milliseconds(infos);
	if (remaining_ms < 0)
		remaining_ms = 0;
	time.tv_sec += remaining_ms / 1000;
	time.tv_nsec += (remaining_ms % 1000) * 1000000;
	if (time.tv_nsec >= 1000000000)
	{
		time.tv_sec += 1;
		time.tv_nsec -= 1000000000;
	}
	return (time);
}
