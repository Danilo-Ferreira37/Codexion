#include <time.h>
#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

typedef struct s_info_simulation t_info_simulation;
typedef struct s_coder t_coder;

typedef struct s_print_locked {
    int (*print)(const char *, ...);
    pthread_mutex_t lock;
} t_print_locked;

typedef struct s_dongle
{
	int dongle_id;
	t_coder *waiting_queue[2];
	int owner;
	int released_ms;
	pthread_mutex_t lock;
	pthread_cond_t  cond;

}	t_dongle;

typedef struct s_coder
{
	int coder_id;
	t_dongle *left_dongle;
	t_dongle *right_dongle;
	struct s_coder *right_coder;

	int last_compile;
	int died;
	int total_compiles;
	pthread_t thread;
}	t_coder;

typedef struct s_supervisor
{
    t_info_simulation *info;
    pthread_t thread;
}   t_supervisor;

// mudar pra long
typedef struct s_info_simulation
{
    int	number_of_coders;
    long	time_to_burnout;
    long	time_to_compile;
    long	time_to_debug;
    long	time_to_refactor;
	int number_of_compiles_required;
	long dongle_cooldown;
	char scheduler;
	
	long start_ms;
	int qnty_coders_comp;
	int running;

	pthread_mutex_t lock;
	t_supervisor *supervisor;
	t_print_locked *printl;
    t_coder  *list_of_coders;
	t_dongle	**dongles;

}   t_info_simulation;

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
int debug_and_refactor(t_coder *self, t_info_simulation *info);
void printl(char *message, t_info_simulation *info, int coder_id);
int stopped(t_info_simulation *info);
t_coder    *scheduler(t_coder **waiting_queue, int remove_chosen_coder, t_info_simulation *infos);

//LIST MANIPULATION
void append_queue(t_coder **queue, t_coder *coder);

//THREADS ROUTINES
void *thread_algoritm(void *infos);
void *supervision(void *information);
void    join_threads(t_info_simulation *info);

//TIME
void time_wait(int time_ms);
int current_milliseconds(t_info_simulation *infos);
