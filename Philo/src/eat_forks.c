/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   eat_forks.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: blohrer <blohrer@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/13 17:47:14 by blohrer           #+#    #+#             */
/*   Updated: 2025/08/05 17:44:55 by blohrer          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

// void	eat(t_philo *philo)
// {
// 	pthread_mutex_lock(philo->left_fork);
// 	if (simulation_should_stop(philo))
// 	{
// 		pthread_mutex_unlock(philo->left_fork);
// 		return ;
// 	}
// 	print_status(philo, "has taken a fork");
// 	if (philo->data->nb_philo == 1)
// 	{
// 		ft_usleep(philo->data->time_to_die, philo->data);
// 		pthread_mutex_unlock(philo->left_fork);
// 		return ;
// 	}
// 	pthread_mutex_lock(philo->right_fork);
// 	if (simulation_should_stop(philo))
// 	{
// 		pthread_mutex_unlock(philo->right_fork);
// 		pthread_mutex_unlock(philo->left_fork);
// 		return ;
// 	}
// 	print_status(philo, "has taken a fork");
// 	do_eating(philo);
// }

// void	do_eating(t_philo *philo)
// {
// 	pthread_mutex_lock(&philo->data->death_lock);
// 	philo->last_meal_time = get_time_in_ms();
// 	philo->meals_eaten++;
// 	pthread_mutex_unlock(&philo->data->death_lock);
// 	if (simulation_should_stop(philo))
// 	{
// 		pthread_mutex_unlock(philo->right_fork);
// 		pthread_mutex_unlock(philo->left_fork);
// 		return ;
// 	}
// 	print_status(philo, "is eating");
// 	ft_usleep(philo->data->time_to_eat, philo->data);
// 	pthread_mutex_unlock(philo->right_fork);
// 	pthread_mutex_unlock(philo->left_fork);
// 	print_status(philo, "is sleeping");
// }
void	eat(t_philo *philo)
{
	// Bestimme Pointer/Index der beiden Forks
	pthread_mutex_t *first;
	pthread_mutex_t *second;

	if (philo->left_fork < philo->right_fork)
	{
		first = philo->left_fork;
		second = philo->right_fork;
	}
	else
	{
		first = philo->right_fork;
		second = philo->left_fork;
	}

	pthread_mutex_lock(first);
	if (simulation_should_stop(philo))
	{
		pthread_mutex_unlock(first);
		return ;
	}
	print_status(philo, "has taken a fork");
	if (philo->data->nb_philo == 1)
	{
		ft_usleep(philo->data->time_to_die, philo->data);
		pthread_mutex_unlock(first);
		return ;
	}
	pthread_mutex_lock(second);
	if (simulation_should_stop(philo))
	{
		pthread_mutex_unlock(second);
		pthread_mutex_unlock(first);
		return ;
	}
	print_status(philo, "has taken a fork");
	do_eating(philo, first, second);
}

void	do_eating(t_philo *philo, pthread_mutex_t *first, pthread_mutex_t *second)
{
	pthread_mutex_lock(&philo->data->death_lock);
	philo->last_meal_time = get_time_in_ms();
	philo->meals_eaten++;
	pthread_mutex_unlock(&philo->data->death_lock);
	if (simulation_should_stop(philo))
	{
		pthread_mutex_unlock(second);
		pthread_mutex_unlock(first);
		return ;
	}
	print_status(philo, "is eating");
	ft_usleep(philo->data->time_to_eat, philo->data);
	pthread_mutex_unlock(second);
	pthread_mutex_unlock(first);
	print_status(philo, "is sleeping");
}

void	print_status(t_philo *philo, char *msg)
{
	long	timestamp;

	pthread_mutex_lock(&philo->data->print_lock);
	if (!simulation_should_stop(philo))
	{
		timestamp = get_time_in_ms() - philo->data->start_time;
		printf("%ld %d %s\n", timestamp, philo->id, msg);
	}
	pthread_mutex_unlock(&philo->data->print_lock);
}
