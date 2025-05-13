/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   monitor.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: blohrer <blohrer@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/08 16:52:08 by blohrer           #+#    #+#             */
/*   Updated: 2025/05/13 17:28:46 by blohrer          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void	*monitor(void *arg)
{
	t_data	*data;
	int		i;

	data = (t_data *)arg;
	while (1)
	{
		i = 0;
		while (i < data->nb_philo)
		{
			if (check_philo_death(data, i))
				return (NULL);
			i++;
		}
		if (check_end_condition(data))
			return (NULL);
		usleep(1000 * 5);
	}
}

int	check_philo_death(t_data *data, int i)
{
	long	now;
	long	time_since_last_meal;

	pthread_mutex_lock(&data->death_lock);
	now = get_time_in_ms();
	time_since_last_meal = now - data->philos[i].last_meal_time;
	if (time_since_last_meal > data->time_to_die)
	{
		pthread_mutex_lock(&data->print_lock);
		printf("%ld %d died\n", now - data->start_time, data->philos[i].id);
		pthread_mutex_unlock(&data->print_lock);
		pthread_mutex_lock(&data->sim_lock);
		data->simulation_active = 0;
		pthread_mutex_unlock(&data->sim_lock);
		pthread_mutex_unlock(&data->death_lock);
		return (1);
	}
	pthread_mutex_unlock(&data->death_lock);
	return (0);
}

int	check_end_condition(t_data *data)
{
	int	result;

	pthread_mutex_lock(&data->death_lock);
	result = (simulation_stopped(data) || (data->must_eat > 0
				&& data->philos_finished >= data->nb_philo));
	pthread_mutex_unlock(&data->death_lock);
	return (result);
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
