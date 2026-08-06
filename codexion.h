#include <time.h>
#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

typedef struct s_dongle
{
	int dongle_id;
	int cooldown;
	int waiting_queue[2];
	int owner;
	pthread_mutex_t lock;
	pthread_cond_t  cond;

}	t_dongle;

typedef struct s_coder
{
	int code_id;
	t_dongle *left_dongle;
	t_dongle *right_dongle;
	struct s_coder *left_coder;
	struct s_coder *right_coder;

	int time_without_copile;
	int total_copiles;
	int	time_to_die;
	int has_left_dongle;
	int has_right_dongle;
	pthread_t thread;
}	t_coder;

typedef struct s_info_simulation
{
    int	number_of_coders;
    int	time_to_burnout;
    int	time_to_copile;
    int	time_to_debug;
    int	time_to_refactor;
	int number_of_compiles_required;
	int dongle_cooldown;
	int start_ms;


	char scheduler;
	int running;
    t_coder  *list_of_coders;
	t_dongle	**dongles;

}   t_info_simulation;

typedef struct s_thread_args {
    t_coder *coder;
    t_info_simulation *infos;
} t_thread_args;
 
//PARSER AND CLEAR ALLOC
int error(char *error_msg);
int	args_parse(char **av);
void	*clear_allocation(t_coder	*coders, t_dongle	**dongles);

//INITS
void *init_info_simulation(t_info_simulation	*info_simulation, char	**av);
t_coder	*init_list_of_coders(t_info_simulation infos);
void create_threads(t_info_simulation *infos);

//ROUTINE
void* thread_algoritm(void *infos);
void    join_threads(t_info_simulation *info);

//TIME
void time_wait(int time_ms);
struct timespec get_cooldwn(t_info_simulation infos);
int current_milliseconds(t_info_simulation *infos);

//TRASH
void compile(t_coder *coder, t_info_simulation infos);
