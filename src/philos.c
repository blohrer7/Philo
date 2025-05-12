/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philos.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: blohrer <blohrer@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/07 15:47:59 by blohrer           #+#    #+#             */
/*   Updated: 2025/05/09 10:45:08 by blohrer          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int	init_philosophers(t_data *data)
{
	int	i;

	if (data->philos == NULL)
		return (-1);
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
	int			i;
	pthread_t	monitor_thread;

	if (!data || !data->nb_philo)
		return (-1);
	i = 0;
	while (i < data->nb_philo)
	{
		if (pthread_create(&data->philos[i].thread, NULL, philo_routine,
				&data->philos[i]) != 0)
			return (write(2, "Error: Failed to create thread.\n", 33), 1);
		i++;
	}
	usleep(1000 * 200);
	if (pthread_create(&monitor_thread, NULL, monitor, data) != 0)
		return (write(2, "Error: Failed to create monitor thread.\n", 41), 1);
	pthread_join(monitor_thread, NULL);
	i = 0;
	while (i < data->nb_philo)
	{
		pthread_join(data->philos[i].thread, NULL);
		i++;
	}
	return (0);
}

void	*philo_routine(void *arg)
{
	t_philo	*philo;

	philo = (t_philo *)arg;
	if ((philo->id % 2) > 0)
		usleep(1000 * 100);
	while (!simulation_should_stop(philo))
	{
		take_forks(philo);
		if (eat(philo))
			return (NULL);
		pthread_mutex_lock(&philo->data->print_lock);
		printf("%ld %d is sleeping\n", get_time_in_ms()
			- philo->data->start_time, philo->id);
		pthread_mutex_unlock(&philo->data->print_lock);
		usleep(philo->data->time_to_sleep * 1000);
		pthread_mutex_lock(&philo->data->print_lock);
		printf("%ld %d is thinking\n", get_time_in_ms()
			- philo->data->start_time, philo->id);
		pthread_mutex_unlock(&philo->data->print_lock);
	}
	return (NULL);
}

void	take_forks(t_philo *philo)
{
	pthread_mutex_lock(philo->left_fork);
	pthread_mutex_lock(&philo->data->print_lock);
	printf("%ld %d has taken a fork\n", get_time_in_ms()
		- philo->data->start_time, philo->id);
	pthread_mutex_unlock(&philo->data->print_lock);
	pthread_mutex_lock(philo->right_fork);
	pthread_mutex_lock(&philo->data->print_lock);
	printf("%ld %d has taken a fork\n", get_time_in_ms()
		- philo->data->start_time, philo->id);
	pthread_mutex_unlock(&philo->data->print_lock);
}

int	eat(t_philo *philo)
{
	pthread_mutex_lock(&philo->data->print_lock);
	printf("%ld %d is eating\n", get_time_in_ms() - philo->data->start_time,
		philo->id);
	pthread_mutex_unlock(&philo->data->print_lock);

	// ✅ Set last_meal_time before sleeping to simulate start of eating
	pthread_mutex_lock(&philo->data->death_lock);
	philo->last_meal_time = get_time_in_ms();
	pthread_mutex_unlock(&philo->data->death_lock);

	usleep(philo->data->time_to_eat * 1000);

	// Continue with meal tracking
	pthread_mutex_lock(&philo->data->death_lock);
	philo->meals_eaten++;
	if (philo->data->must_eat > 0
		&& philo->meals_eaten >= philo->data->must_eat)
	{
		philo->data->philos_finished++;
		if (philo->data->philos_finished >= philo->data->nb_philo)
			philo->data->someone_died = 1;
		pthread_mutex_unlock(&philo->data->death_lock);
		pthread_mutex_unlock(philo->left_fork);
		pthread_mutex_unlock(philo->right_fork);
		return (1);
	}
	pthread_mutex_unlock(&philo->data->death_lock);
	pthread_mutex_unlock(philo->left_fork);
	pthread_mutex_unlock(philo->right_fork);
	return (0);
}
