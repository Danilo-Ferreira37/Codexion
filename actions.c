/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   actions.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dosorio- <dosorio-@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/24 15:53:17 by dosorio-          #+#    #+#             */
/*   Updated: 2026/08/25 20:50:38 by dosorio-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

static int	compile(t_coder *self, t_info_simulation *info)
{
	printl("%d %d is compiling\n", info, self->coder_id);
	pthread_mutex_lock(&self->thread_lock);
	self->last_compile = current_milliseconds(info);
	self->total_compiles++;
	pthread_mutex_unlock(&self->thread_lock);
	times_wait(info->time_to_compile);
	pthread_mutex_lock(&self->right_dongle->lock);
	self->right_dongle->owner = 0;
	self->right_dongle->released_ms = current_milliseconds(info);
	pthread_cond_broadcast(&self->right_dongle->cond);
	pthread_mutex_unlock(&self->right_dongle->lock);
	pthread_mutex_lock(&self->left_dongle->lock);
	self->left_dongle->owner = 0;
	self->left_dongle->released_ms = current_milliseconds(info);
	pthread_cond_broadcast(&self->left_dongle->cond);
	pthread_mutex_unlock(&self->left_dongle->lock);
	return (1);
}

static void	view_scheduler(t_coder *self, t_dongle *dongle,
		t_info_simulation *info)
{
	if (scheduler(dongle->waiting_queue, info, 0) == self && !dongle->owner
		&& (current_milliseconds(info) > dongle->released_ms
			+ info->dongle_cooldown || !dongle->released_ms))
		get_dongle(scheduler(dongle->waiting_queue, info, 1)->coder_id, dongle,
			info);
	else
	{
		append_queue(dongle->waiting_queue, self);
		if (dongle->owner)
			pthread_cond_wait(&dongle->cond, &dongle->lock);
	}
}

static int	try_get_dongle(t_coder *self, t_dongle *dongle,
		t_info_simulation *info)
{
	if (stopped(info))
		return (-1);
	if (!dongle)
		return (0);
	pthread_mutex_lock(&dongle->lock);
	if (dongle->owner == self->coder_id)
	{
		pthread_mutex_unlock(&dongle->lock);
		return (0);
	}
	else if (!dongle->owner && !dongle->waiting_queue[0]
		&& (current_milliseconds(info) > dongle->released_ms
			+ info->dongle_cooldown || !dongle->released_ms))
		get_dongle(self->coder_id, dongle, info);
	else
		view_scheduler(self, dongle, info);
	pthread_mutex_unlock(&dongle->lock);
	return (stopped(info));
}

static int	verify_compile(t_coder *self, t_dongle *first, t_dongle *second,
		t_info_simulation *info)
{
	if (stopped(info))
		return (-1);
	pthread_mutex_lock(&first->lock);
	pthread_mutex_lock(&second->lock);
	if (self->coder_id == first->owner && self->coder_id == second->owner)
	{
		pthread_mutex_unlock(&second->lock);
		pthread_mutex_unlock(&first->lock);
		return (compile(self, info));
	}
	pthread_mutex_unlock(&second->lock);
	pthread_mutex_unlock(&first->lock);
	return (0);
}

int	try_compile(t_coder *self, t_info_simulation *info)
{
	t_dongle	*first;
	t_dongle	*second;

	if (stopped(info))
		return (-1);
	if (!self->left_dongle || !self->right_dongle)
		return (0);
	if (self->left_dongle->dongle_id < self->right_dongle->dongle_id)
	{
		first = self->left_dongle;
		second = self->right_dongle;
	}
	else
	{
		first = self->right_dongle;
		second = self->left_dongle;
	}
	if (try_get_dongle(self, first, info))
		return (-1);
	if (try_get_dongle(self, second, info))
		return (-1);
	return (verify_compile(self, first, second, info));
}
