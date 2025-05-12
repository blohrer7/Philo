/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   monitor.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: blohrer <blohrer@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/08 16:52:08 by blohrer           #+#    #+#             */
/*   Updated: 2025/05/09 10:45:57 by blohrer          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void	*monitor(void *arg)
{
	t_data	*data;
	int		i;
	long	now;

	data = (t_data *)arg;
	while (1)
	{
		i = 0;
		while (i < data->nb_philo)
		{
			pthread_mutex_lock(&data->death_lock);
			now = get_time_in_ms();
			if (now - data->philos[i].last_meal_time > data->time_to_die)
			{
				pthread_mutex_lock(&data->print_lock);
				printf("DEBUG: Philosopher %d last_meal = %ld, now = %ld\n",data->philos[i].id,data->philos[i].last_meal_time,now);
				printf("%ld %d died\n", now - data->start_time,
					data->philos[i].id);
				pthread_mutex_unlock(&data->print_lock);
				data->someone_died = 1;
				pthread_mutex_unlock(&data->death_lock);
				return (NULL);
			}
			pthread_mutex_unlock(&data->death_lock);
			i++;
		}
		usleep(1000);
	}
	return (NULL);
}
