/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: blohrer <blohrer@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/07 07:58:39 by blohrer           #+#    #+#             */
/*   Updated: 2025/05/12 20:29:10 by blohrer          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int	check_input(int argc, char *argv[])
{
	if (argc < 5 || argc > 6)
		return (write(2, "Error: Wrong size of arguments\n", 33), -1);
	if (ft_atoi(argv[1]) < 0 || ft_atoi(argv[1]) > 200)
		return (write(2, "Error: No philo or over 200 philos.\n", 38), -1);
	if (ft_atoi(argv[2]) < 0)
		return (write(2, "Error: Wrong time to eat.\n", 28), -1);
	if (ft_atoi(argv[3]) < 0)
		return (write(2, "Error: Wrong time to sleep.\n", 30), -1);
	if (ft_atoi(argv[4]) < 0)
		return (write(2, "Error: Wrong time to die.\n", 28), -1);
	if (argc == 6 && ft_atoi(argv[5]) < 0)
		return (write(2, "Error: Wrong number of meals.\n", 32), -1);
	return (0);
}

int	init_mutexes(t_data *data)
{
	if (pthread_mutex_init(&data->print_lock, NULL) != 0)
	{
		free(data->forks);
		return (write(2, "Error: Failed to initialize print_lock\n", 40), -1);
	}
	if (pthread_mutex_init(&data->death_lock, NULL) != 0)
	{
		free(data->forks);
		pthread_mutex_destroy(&data->print_lock);
		return (write(2, "Error: Failed to initialize death_lock\n", 40), -1);
	}
	return (0);
}

int	init_data(t_data *data, int argc, char *argv[])
{
	data->simulation_active = 1;
	pthread_mutex_init(&data->sim_lock, NULL);
	data->nb_philo = ft_atoi(argv[1]);
	data->time_to_eat = ft_atoi(argv[2]);
	data->time_to_sleep = ft_atoi(argv[3]);
	data->time_to_die = ft_atoi(argv[4]);
	if (argc == 6)
		data->must_eat = ft_atoi(argv[5]);
	else
		data->must_eat = -1;
	if ((data->start_time = get_time_in_ms()) == -1)
		return (write(2, "Error: Failed to get start time\n", 33), -1);
	data->forks = malloc(sizeof(pthread_mutex_t) * data->nb_philo);
	if (data->forks == NULL)
		return (write(2, "Error: Failed allocate memory for forks\n", 41), -1);
	return (init_mutexes(data));
}

long	get_time_in_ms(void)
{
	struct timespec	ts;

	if (clock_gettime(CLOCK_MONOTONIC, &ts) == -1)
	{
		write(2, "clock_gettime failed", 20);
		return (-1);
	}
	return (ts.tv_sec * 1000 + ts.tv_nsec / 1000000);
}
