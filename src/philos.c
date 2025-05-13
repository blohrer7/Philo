/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philos.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: blohrer <blohrer@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/07 15:47:59 by blohrer           #+#    #+#             */
/*   Updated: 2025/05/13 17:56:34 by blohrer          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int	init_philosophers(t_data *data)
{
	int	i;

	if (data->philos == NULL)
		return (-1);
	data->start_time = get_time_in_ms();
	i = 0;
	while (i < data->nb_philo)
	{
		data->philos[i].id = i + 1;
		data->philos[i].left_fork = &data->forks[i];
		data->philos[i].right_fork = &data->forks[(i + 1) % data->nb_philo];
		data->philos[i].last_meal_time = data->start_time;
		data->philos[i].meals_eaten = 0;
		data->philos[i].data = data;
		i++;
	}
	return (0);
}

int	start_philosophers(t_data *data)
{
	if (!data || !data->nb_philo)
		return (-1);
	data->start_time = get_time_in_ms();
	if (data->nb_philo == 1)
		return (handle_one_philosopher(data));
	return (start_all_philosopher_threads(data));
}

int	handle_one_philosopher(t_data *data)
{
	data->start_time = get_time_in_ms();
	pthread_mutex_lock(&data->print_lock);
	printf("0 1 is thinking\n");
	pthread_mutex_unlock(&data->print_lock);
	ft_usleep(100);
	pthread_mutex_lock(&data->print_lock);
	printf("100 1 has taken a fork\n");
	pthread_mutex_unlock(&data->print_lock);
	ft_usleep(data->time_to_die - 100);
	pthread_mutex_lock(&data->print_lock);
	printf("%ld 1 died\n", get_time_in_ms() - data->start_time);
	pthread_mutex_unlock(&data->print_lock);
	return (0);
}

int	start_all_philosopher_threads(t_data *data)
{
	int			i;
	pthread_t	monitor_thread;

	i = 0;
	while (i < data->nb_philo)
	{
		data->philos[i].last_meal_time = data->start_time;
		if (pthread_create(&data->philos[i].thread, NULL,
				philo_routine, &data->philos[i]) != 0)
			return (write(2, "Error: Failed to create thread.\n", 33), 1);
		usleep(100);
		i++;
	}
	if (pthread_create(&monitor_thread, NULL, monitor, data) != 0)
		return (write(2, "Error: Failed to create monitor thread.\n", 41), 1);
	pthread_join(monitor_thread, NULL);
	i = 0;
	while (i < data->nb_philo)
		pthread_join(data->philos[i++].thread, NULL);
	return (0);
}

void	*philo_routine(void *arg)
{
	t_philo	*philo;

	philo = (t_philo *)arg;
	if (philo->id % 2 == 0)
		usleep(philo->data->time_to_eat * 1000);
	while (!simulation_should_stop(philo))
	{
		take_forks(philo);
		if (eat(philo) != 0)
			break ;
		if (simulation_should_stop(philo))
			break ;
		pthread_mutex_lock(&philo->data->print_lock);
		printf("%ld %d is sleeping\n", get_time_in_ms()
			- philo->data->start_time, philo->id);
		pthread_mutex_unlock(&philo->data->print_lock);
		usleep(philo->data->time_to_sleep * 1000);
		if (simulation_should_stop(philo))
			break ;
		pthread_mutex_lock(&philo->data->print_lock);
		printf("%ld %d is thinking\n", get_time_in_ms()
			- philo->data->start_time, philo->id);
		pthread_mutex_unlock(&philo->data->print_lock);
	}
	return (NULL);
}
