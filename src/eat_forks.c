/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   eat_forks.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: blohrer <blohrer@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/13 17:47:14 by blohrer           #+#    #+#             */
/*   Updated: 2025/05/13 17:49:11 by blohrer          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void	take_forks(t_philo *philo)
{
	if (philo->id % 2 == 0)
	{
		lock_and_print(philo, philo->right_fork, "right");
		lock_and_print(philo, philo->left_fork, "left");
	}
	else
	{
		lock_and_print(philo, philo->left_fork, "left");
		lock_and_print(philo, philo->right_fork, "right");
	}
}

void	lock_and_print(t_philo *philo, pthread_mutex_t *fork, char *side)
{
	long	timestamp;

	pthread_mutex_lock(fork);
	timestamp = get_time_in_ms();
	pthread_mutex_lock(&philo->data->print_lock);
	printf("%ld %d has taken %s fork\n", timestamp - philo->data->start_time,
		philo->id, side);
	pthread_mutex_unlock(&philo->data->print_lock);
}

int	eat(t_philo *philo)
{
	long	start;

	start = get_time_in_ms();
	update_meal_status_and_unlock(philo, start);
	print_status(philo, "is eating", start);
	ft_usleep(philo->data->time_to_eat);
	if (philo->data->must_eat > 0
		&& philo->meals_eaten >= philo->data->must_eat)
		return (1);
	return (0);
}

void	update_meal_status_and_unlock(t_philo *philo, long timestamp)
{
	pthread_mutex_lock(&philo->data->death_lock);
	philo->last_meal_time = timestamp;
	philo->meals_eaten++;
	if (philo->data->must_eat > 0
		&& philo->meals_eaten == philo->data->must_eat)
		philo->data->philos_finished++;
	pthread_mutex_unlock(&philo->data->death_lock);
	pthread_mutex_unlock(philo->left_fork);
	pthread_mutex_unlock(philo->right_fork);
}

void	print_status(t_philo *philo, char *msg, long timestamp)
{
	pthread_mutex_lock(&philo->data->print_lock);
	printf("%ld %d %s\n", timestamp - philo->data->start_time, philo->id, msg);
	pthread_mutex_unlock(&philo->data->print_lock);
}
