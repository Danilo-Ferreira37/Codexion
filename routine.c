/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   routine.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dosorio- <dosorio-@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/24 15:52:53 by dosorio-          #+#    #+#             */
/*   Updated: 2026/08/27 14:46:31 by dosorio-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

static void	thread_algoritm_loop(t_info_simulation *info, t_coder *self)
{
	int	cond;

	while (1)
	{
		cond = try_compile(self, info);
		if (cond == -1)
			break ;
		pthread_mutex_lock(&info->lock);
		if (self->total_compiles == info->number_of_compiles_required
			|| !info->running)
		{
			pthread_mutex_unlock(&info->lock);
			break ;
		}
		pthread_mutex_unlock(&info->lock);
		if (cond)
			if (debug_and_refactor(self, info))
				break ;
	}
}

static void	*thread_algoritm(void *args)
{
	t_thread_args		*thread_args;
	t_info_simulation	*info;
	t_coder				*self;

	thread_args = (t_thread_args *)args;
	self = thread_args->coder;
	info = thread_args->infos;
	free(args);
	if (verify_1_coder(self, info))
		return (NULL);
	thread_algoritm_loop(info, self);
	pthread_mutex_lock(&info->lock);
	info->qnty_coders_comp++;
	pthread_mutex_unlock(&info->lock);
	return (NULL);
}

static int	thread_burnout(t_info_simulation *info, t_coder *coder)
{
	int	burnout;

	burnout = 0;
	pthread_mutex_lock(&info->lock);
	pthread_mutex_lock(&coder->thread_lock);
	if (coder->total_compiles != info->number_of_compiles_required)
		burnout = current_milliseconds(info)
			- coder->last_compile >= info->time_to_burnout;
	pthread_mutex_unlock(&coder->thread_lock);
	if (burnout || info->qnty_coders_comp == info->number_of_coders)
	{
		if (burnout)
			printf("%d %d burned out\n", current_milliseconds(info),
				coder->coder_id);
		info->running = 0;
		pthread_mutex_unlock(&info->lock);
		return (-1);
	}
	pthread_mutex_unlock(&info->lock);
	return (0);
}

static void	*supervision(void *information)
{
	t_info_simulation	*info;
	t_coder				*coder;

	info = (t_info_simulation *)information;
	coder = info->list_of_coders;
	while (coder)
	{
		if (thread_burnout(info, coder))
			break ;
		coder = coder->right_coder;
		times_wait(1);
	}
	return (wakeup_all_threads(info->dongles));
}

int	create_threads(t_info_simulation *infos)
{
	t_coder			*thread;
	t_thread_args	*args;

	pthread_create(&infos->supervisor->thread, NULL, supervision, infos);
	thread = infos->list_of_coders;
	while (1)
	{
		args = malloc(sizeof(t_thread_args));
		if (!args)
			return (error(NULL, infos));
		args->coder = thread;
		args->infos = infos;
		pthread_create(&thread->thread, NULL, thread_algoritm, args);
		if (thread->coder_id == infos->number_of_coders)
			break ;
		thread = thread->right_coder;
	}
	return (0);
}
