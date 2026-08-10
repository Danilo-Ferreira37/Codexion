#include <time.h>
#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

typedef struct s_print_locked {
    int (*print)(const char *, ...);
    pthread_mutex_t lock;
} t_print_locked;

typedef struct s_dongle
{
	int dongle_id;
	int waiting_queue[2];
	int owner;
	int released_ms;
	pthread_mutex_t lock;
	pthread_cond_t  cond;

}	t_dongle;

typedef struct s_coder
{
	int code_id;
	t_dongle *left_dongle;
	t_dongle *right_dongle;
	struct s_coder *right_coder;

	int time_without_copile;
	int total_compiles;
    int died;
	pthread_t thread;
}	t_coder;

typedef struct s_info_simulation
{
    int	number_of_coders;
    int	time_to_burnout;
    int	time_to_compile;
    int	time_to_debug;
    int	time_to_refactor;
	int number_of_compiles_required;
	int dongle_cooldown;
	char scheduler;
	
	int start_ms;
	int running;
	t_print_locked *printl;
    t_coder  *list_of_coders;
	t_dongle	**dongles;

}   t_info_simulation;

typedef struct s_supervisor
{
    t_info_simulation *info;
    pthread_t thread;
}   t_supervisor;

typedef struct s_thread_args {
    t_coder *coder;
    t_info_simulation *infos;
}   t_thread_args;

//PARSER AND CLEAR ALLOC
int error(char *error_msg);
int	args_parse(char **av);
void	*clear_allocation(t_info_simulation *infos);

//INITS
void *init_info_simulation(t_info_simulation	*info_simulation, char	**av);
t_coder	*init_list_of_coders(t_info_simulation infos);
void create_threads(t_info_simulation *infos);

//ACTIONS
int try_compile(t_coder *self, t_info_simulation *info);
void try_get_dongle(int coder_id, t_dongle *dongle, t_info_simulation *info);
void debug(t_coder *self, t_info_simulation *info);
void refactor(t_coder *self, t_info_simulation *info);

//LIST MANIPULATION
void append_queue(int *queue, int coder_id);
int popleft_queue(int *queue);

//THREADS ROUTINES
void* thread_algoritm(void *infos);
void *supervision(void *information);
void    join_threads(t_info_simulation *info);

//TIME
void time_wait(int time_ms);
struct timespec get_cooldwn(t_info_simulation infos);
int current_milliseconds(t_info_simulation *infos);
