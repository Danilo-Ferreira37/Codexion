#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

typedef struct s_node
{
	void		 *data;
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
	int time_without_copile;
	int total_copiles;
	int	time_to_die;

}	t_coder;


typedef struct s_scheduler
{
	char *type_scheduler;
}	t_scheduler;


typedef struct s_simulation
{
    int	number_of_coders;
    int	time_to_burnout;
    int	time_to_copile;
    int	time_to_debug;
    int	time_to_refactor;
	int number_of_compiles_required;
	int dongle_cooldown;

	t_coder *coders;

}   t_simulation;

int error(char *error_msg);
int args_parse(char **av);
void get_init_info(char **av, t_node *info_list);