/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philos.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: blohrer <blohrer@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/07 15:47:59 by blohrer           #+#    #+#             */
/*   Updated: 2025/05/22 17:43:58 by blohrer          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int	init_philosophers(t_data *data)
{
	int		i;
	long	start;

	i = 0;
	data->philos = malloc(sizeof(t_philo) * data->nb_philo);
	if (!data->philos)
		return (write(2, "Error: malloc philos\n", 20), -1);
	start = get_time_in_ms();
	while (i < data->nb_philo)
	{
		data->philos[i].id = i + 1;
		data->philos[i].left_fork = &data->forks[i];
		data->philos[i].right_fork = &data->forks[(i + 1) % data->nb_philo];
		data->philos[i].last_meal_time = start;
		data->philos[i].meals_eaten = 0;
		data->philos[i].data = data;
		i++;
	}
	return (0);
}

int	handle_one_philosopher(t_data *data)
{
	data->start_time = get_time_in_ms();
	pthread_mutex_lock(&data->print_lock);
	printf("0 1 is thinking\n");
	pthread_mutex_unlock(&data->print_lock);
	ft_usleep(100, data);
	pthread_mutex_lock(&data->print_lock);
	printf("100 1 has taken a fork\n");
	pthread_mutex_unlock(&data->print_lock);
	ft_usleep(data->time_to_die - 100, data);
	pthread_mutex_lock(&data->print_lock);
	printf("%ld 1 died\n", get_time_in_ms() - data->start_time);
	pthread_mutex_unlock(&data->print_lock);
	return (0);
}

void	*philo_routine(void *arg)
{
	t_philo	*philo;

	philo = (t_philo *)arg;
	usleep((philo->id % 10) * 200);
	if (philo->id % 2 == 0 || philo->id == philo->data->nb_philo)
	{
		philo_think(philo);
		ft_usleep(philo->data->time_to_eat / 2, philo->data);
	}
	philo_life_loop(philo);
	return (NULL);
}

void	philo_life_loop(t_philo *philo)
{
	while (!simulation_should_stop(philo))
	{
		if (philo->data->must_eat > 0
			&& philo->meals_eaten >= philo->data->must_eat)
			break ;
		eat(philo);
		if (philo->data->must_eat > 0
			&& philo->meals_eaten >= philo->data->must_eat)
			break ;
		if (simulation_should_stop(philo))
			break ;
		philo_sleep(philo);
		if (simulation_should_stop(philo))
			break ;
		philo_think(philo);
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
