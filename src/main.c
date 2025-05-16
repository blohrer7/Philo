/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: blohrer <blohrer@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/06 15:57:49 by blohrer           #+#    #+#             */
/*   Updated: 2025/05/16 10:18:23 by blohrer          ###   ########.fr       */
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
	{
		free(data.philos);
		return (1);
	}
	if (start_philosophers(&data) != 0)
	{
		cleanup(&data);
		return (1);
	}
	cleanup(&data);
	return (0);
}

void	cleanup(t_data *data)
{
	int	i;

	i = 0;
	while (i < data->nb_philo)
	{
		pthread_mutex_destroy(&data->forks[i]);
		i++;
	}
	pthread_mutex_destroy(&data->print_lock);
	pthread_mutex_destroy(&data->death_lock);
	pthread_mutex_destroy(&data->sim_lock);
	free(data->forks);
	free(data->philos);
}

void	ft_usleep(long ms, t_data *data)
{
	long start = get_time_in_ms();
    while (is_simulation_active(data))
    {
        if (get_time_in_ms() - start >= ms)
            break;
        usleep(500); // Kurze Pause, damit du Stop-Flags checken kannst
    }
}

int	init_mutexes(t_data *data)
{
	if (init_fork_mutexes(data) != 0)
		return (-1);
	if (init_global_mutexes(data) != 0)
		return (-1);
	return (0);
}
