/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   monitor.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: blohrer <blohrer@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/08 16:52:08 by blohrer           #+#    #+#             */
/*   Updated: 2025/05/19 16:24:34 by blohrer          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void	*monitor(void *arg)
{
	t_data	*data;
	int		i;
	int		full;

	data = (t_data *)arg;
	while (1)
	{
		i = 0;
		full = 0;
		while (i < data->nb_philo)
		{
			if (monitor_check_death_or_full(data, &full, i))
				return (NULL);
			i++;
		}
		if (data->must_eat > 0 && full == data->nb_philo)
		{
			pthread_mutex_lock(&data->death_lock);
			data->simulation_active = 0;
			pthread_mutex_unlock(&data->death_lock);
			return (NULL);
		}
		usleep(1000);
	}
	return (NULL);
}

int	monitor_check_death_or_full(t_data *data, int *full, int i)
{
	long	now;

	pthread_mutex_lock(&data->death_lock);
	now = get_time_in_ms();
	if ((now - data->philos[i].last_meal_time) > data->time_to_die)
	{
		pthread_mutex_lock(&data->print_lock);
		data->simulation_active = 0;
		printf("%ld %d died\n", now - data->start_time, data->philos[i].id);
		pthread_mutex_unlock(&data->print_lock);
		pthread_mutex_unlock(&data->death_lock);
		return (1);
	}
	if (data->must_eat > 0 && data->philos[i].meals_eaten >= data->must_eat)
		(*full)++;
	pthread_mutex_unlock(&data->death_lock);
	return (0);
}

int	is_simulation_active(t_data *data)
{
	int	status;

	pthread_mutex_lock(&data->sim_lock);
	status = data->simulation_active;
	pthread_mutex_unlock(&data->sim_lock);
	return (status);
}
