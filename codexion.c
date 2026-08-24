/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   codexion.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dosorio- <dosorio-@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/24 15:53:13 by dosorio-          #+#    #+#             */
/*   Updated: 2026/08/24 21:03:48 by dosorio-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

int	main(int ac, char **av)
{
	t_info_simulation	info_simulation;

	if (ac != 9)
		return (error("Invalid number of arguments!", NULL));
	if (args_parse(av))
		return (-1);
	if (!init_info_simulation(&info_simulation, av))
		return (error("Memory allocation failure!", NULL));
	info_simulation.list_of_coders = init_list_of_coders(info_simulation);
	if (!info_simulation.list_of_coders)
		return (error("Memory allocation failure!", NULL));
	if (create_threads(&info_simulation))
		return (error("Memory allocation failure!", &info_simulation));
	join_threads(&info_simulation);
	clear_allocation(&info_simulation);
	return (0);
}
