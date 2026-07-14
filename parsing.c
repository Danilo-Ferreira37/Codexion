#include "codexion.h"

int error(char *error_msg)
{
    fprintf(stderr, "Error: %s\n", error_msg);
	return (-1);
}

static int error_in_num(char *arg)
{
	int i;

	i = 0;
	if (arg[i] == '+' || arg[i] == '-')
		i++;
	if (!arg[i])
		return (error("The program received a invalid number"));
	while (arg[i])
	{
		if (arg[i] < '0' || arg[i] > '9') 
			return (error("The program received a invalid number"));
		i++;
	}
	return (0);
}

int args_parse(char **av)
{
    int i;

    i = 1;
    while (i < 8)
    {
		if (error_in_num(av[i++]))
			return (1);
    }
	if (strcmp(av[i], "fifo") && strcmp(av[i], "edf"))
		return (error("The program received a invalid scheduling! (Must be exactly 'fifo' or 'edf')"));
	return (0);
}

/* static t_node *create_node(void *value)
{
	t_node *node;

	node = malloc(sizeof(t_node));
	if (!node)
	{
		error("Memory allocaation failure!");
		return (NULL);
	}
	memset(node, 0, sizeof(t_node));
	node->data = value;
	node->next = NULL;
	return (node);
}

static int node_add_back(char *value, t_node *node)
{
	
}

int get_init_info(char **av, t_node *info_list)
{
	int i;

	i = 1;
	while (i < 8)
		if (node_add_back(av[i++], &info_list));
			return (-1);

} */