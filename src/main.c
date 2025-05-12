/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: blohrer <blohrer@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/06 15:57:49 by blohrer           #+#    #+#             */
/*   Updated: 2025/05/08 17:50:32 by blohrer          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int	main(int argc, char **argv)
{
	t_data	data;

	if (check_input(argc, argv) != 0)
		return (1);
	if (init_data(&data, argc, argv) != 0)
		return (1);
	data.philos = malloc(sizeof(t_philo) * data.nb_philo);
	if (!data.philos)
		return (write(2, "Error: Failed to allocate philosophers\n", 39), 1);
	if (init_philosophers(&data) != 0)
		return (1);
	if (start_philosophers(&data) != 0)
		return (1);
	return (0);
}
