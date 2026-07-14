#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

typedef struct s_node
{
	void	*data;
	struct s_node	*next;
}				   t_node;

typedef struct s_dongle
{
	int dongle_id;
	int cooldwn;
	t_node	*waiting_queue;
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

	char scheduler;

}   t_info_simulation;

int error(char *error_msg);
int	args_parse(char **av);
void    init_info_simulation(t_info_simulation *info_simulation, char **av);
t_node	*init_list_of_coders(int number_of_coders, t_info_simulation infos);
