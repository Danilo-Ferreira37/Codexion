#include <time.h>
#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

typedef struct s_dongle
{
	int dongle_id;
	int cooldwn;
	pthread_mutex_t lock;
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
	int has_l_dongle;
	int has_r_dongle;
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
    t_coder  *list_of_coders;
	t_dongle	**dongles;

}   t_info_simulation;

int error(char *error_msg);
int	args_parse(char **av);
void *init_info_simulation(t_info_simulation	*info_simulation, char	**av);
t_coder	*init_list_of_coders(t_info_simulation infos);
void	*clear_allocation(t_coder	*coders, t_dongle	**dongles);

void fifo(t_coder *list_of_coders, t_info_simulation info);
void edf(t_coder *list_of_coders, t_info_simulation info);

void time_wait(int time_ms);
void compile(t_coder *coder, t_info_simulation infos);
