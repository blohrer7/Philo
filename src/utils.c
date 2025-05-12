/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: blohrer <blohrer@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/07 10:21:03 by blohrer           #+#    #+#             */
/*   Updated: 2025/05/09 10:45:33 by blohrer          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int	ft_atoi(const char *str)
{
	int	result;
	int	sign;

	result = 0;
	sign = 1;
	while (*str == ' ' || *str == '\t' || *str == '\n' || *str == '\v'
		|| *str == '\f' || *str == '\r')
	{
		str++;
	}
	if (*str == '-' || *str == '+')
	{
		if (*str == '-')
		{
			sign = -1;
		}
		str++;
	}
	while (*str >= '0' && *str <= '9')
	{
		result = result * 10 + (*str - '0');
		str++;
	}
	return (sign * result);
}

int	is_valid_number(char *str)
{
	int	i;

	i = 0;
	if (str == NULL || str[0] == '\0')
		return (0);
	while (str[i] != '\0')
	{
		if (str[i] < '0' || str[i] > '9')
			return (0);
		i++;
	}
	return (1);
}

int	simulation_should_stop(t_philo *philo)
{
	int	stop = 0;

	pthread_mutex_lock(&philo->data->death_lock);

	if (philo->data->someone_died)
		stop = 1;
	else if (philo->data->must_eat > 0 &&
			philo->meals_eaten >= philo->data->must_eat)
		stop = 1;

	pthread_mutex_unlock(&philo->data->death_lock);

	return (stop);
}

void	cleanup(t_data *data)
{
	int	i;

	i = 0;
	while(i < data->nb_philo)
	{
		pthread_mutex_destroy(&data->forks[i]);
		i++;
	}
	pthread_mutex_destroy(&data->print_lock);
	pthread_mutex_destroy(&data->death_lock);
	free(data->forks);
	free(data->philos);
}

void	ft_usleep(long ms)
{
	long	start;

	start = get_time_in_ms();
	while (get_time_in_ms() - start < ms)
		usleep(500); // kleinere Sleeps für Genauigkeit
}
