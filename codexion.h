/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   codexion.h                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dosorio- <dosorio-@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/24 21:03:32 by dosorio-          #+#    #+#             */
/*   Updated: 2026/08/27 17:54:49 by dosorio-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h>

typedef struct s_info_simulation	t_info_simulation;
typedef struct s_coder				t_coder;

typedef struct s_print_locked
{
	int								(*print)(const char *, ...);
	pthread_mutex_t					lock;
}									t_print_locked;

typedef struct s_dongle
{
	int								dongle_id;
	t_coder							*waiting_queue[2];
	int								owner;
	long							released_ms;
	pthread_mutex_t					lock;
	pthread_cond_t					cond;

}									t_dongle;

typedef struct s_coder
{
	int								coder_id;
	t_dongle						*left_dongle;
	t_dongle						*right_dongle;
	struct s_coder					*right_coder;

	long							last_compile;
	int								total_compiles;
	pthread_t						thread;
	pthread_mutex_t					thread_lock;
}									t_coder;

typedef struct s_supervisor
{
	t_info_simulation				*info;
	pthread_t						thread;
}									t_supervisor;

typedef struct s_info_simulation
{
	int								number_of_coders;
	long							time_to_burnout;
	long							time_to_compile;
	long							time_to_debug;
	long							time_to_refactor;
	int								number_of_compiles_required;
	long							dongle_cooldown;
	char							scheduler;

	long							start_ms;
	int								qnty_coders_comp;
	int								running;

	pthread_mutex_t					lock;
	t_supervisor					*supervisor;
	t_print_locked					*printl;
	t_coder							*list_of_coders;
	t_dongle						**dongles;

}									t_info_simulation;

typedef struct s_thread_args
{
	t_coder							*coder;
	t_info_simulation				*infos;
}									t_thread_args;

// PARSER AND CLEAR ALLOCATION
int					error(char *error_msg,
						t_info_simulation *infos);
int					args_parse(char **av);
void				*clear_allocation(t_info_simulation *infos);

// INIT STRUCTS
void				*init_info_simulation(t_info_simulation *info_simulation,
						char **av);
t_coder				*init_list_of_coders(t_info_simulation infos);
t_coder				*create_coder(t_info_simulation infos,
						int coder_id);

// THREADS
int					create_threads(t_info_simulation *infos);
void				join_threads(t_info_simulation *info);
void				*wakeup_all_threads(t_dongle **dongles);
void				get_dongle(int coder_id, t_dongle *dongle,
						t_info_simulation *info);
int					verify_1_coder(t_coder *self,
						t_info_simulation *info);

// ACTIONS
int					try_compile(t_coder *self,
						t_info_simulation *info);
int					debug_and_refactor(t_coder *self,
						t_info_simulation *info);
void				printl(char *message,
						t_info_simulation *info, int coder_id);
int					stopped(t_info_simulation *info);
t_coder				*scheduler(t_coder **waiting_queue,
						t_info_simulation *infos,
						int remove_chosen_coder);

// LIST MANIPULATION
void				append_queue(t_coder **queue,
						t_coder *coder);
int					lst_append_new_coders(t_coder *coder,
						t_info_simulation infos, int id_coder);

// TIME
void				times_wait(int time_ms);
int					current_milliseconds(t_info_simulation *infos);
struct timespec		get_cooldown_deadline(t_dongle *dongle,
						t_info_simulation *infos);
