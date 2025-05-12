/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   monitor.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: blohrer <blohrer@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/08 16:52:08 by blohrer           #+#    #+#             */
/*   Updated: 2025/05/12 20:42:31 by blohrer          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"


void	*monitor(void *arg)
{
	t_data	*data;
	int		i;
	long	now;
	long	last_meal;
	long	time_since_last_meal;

	data = (t_data *)arg;

	while (1)
	{
		i = 0;
		while (i < data->nb_philo)
		{
			pthread_mutex_lock(&data->death_lock);
			now = get_time_in_ms();
			last_meal = data->philos[i].last_meal_time;
			time_since_last_meal = now - last_meal;

			if (time_since_last_meal > data->time_to_die)
			{
				pthread_mutex_lock(&data->print_lock);
				printf("%ld %d died\n", now - data->start_time, data->philos[i].id);
				pthread_mutex_unlock(&data->print_lock);

				pthread_mutex_lock(&data->sim_lock);
				data->simulation_active = 0;
				pthread_mutex_unlock(&data->sim_lock);

				pthread_mutex_unlock(&data->death_lock);
				return (NULL);
			}
			pthread_mutex_unlock(&data->death_lock);
			i++;
		}

		pthread_mutex_lock(&data->death_lock);
		if (simulation_stopped(data)
			|| (data->must_eat > 0 && data->philos_finished >= data->nb_philo))
		{
			pthread_mutex_unlock(&data->death_lock);
			return (NULL);
		}
		pthread_mutex_unlock(&data->death_lock);

		usleep(1000 * 5);
	}
}



int	is_simulation_active(t_data *data)
{
	int	status;

	pthread_mutex_lock(&data->sim_lock);
	status = data->simulation_active;
	pthread_mutex_unlock(&data->sim_lock);
	return (status);
}

int	check_meal_completion(t_data *data)
{
	int	i;
	int	finished;

	if (data->must_eat <= 0)
		return (0);
	finished = 0;
	pthread_mutex_lock(&data->death_lock);
	i = 0;
	while (i < data->nb_philo)
	{
		if (data->philos[i].meals_eaten >= data->must_eat)
			finished++;
		i++;
	}
	pthread_mutex_unlock(&data->death_lock);
	if (finished == data->nb_philo)
	{
		pthread_mutex_lock(&data->sim_lock);
		data->simulation_active = 0;
		pthread_mutex_unlock(&data->sim_lock);
		return (1);
	}
	return (0);
}
