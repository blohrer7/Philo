/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: blohrer <blohrer@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/07 10:21:03 by blohrer           #+#    #+#             */
/*   Updated: 2025/05/13 17:50:56 by blohrer          ###   ########.fr       */
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
	return (!is_simulation_active(philo->data));
}

int	simulation_stopped(t_data *data)
{
	int	active;

	pthread_mutex_lock(&data->sim_lock);
	active = data->simulation_active;
	pthread_mutex_unlock(&data->sim_lock);
	return (active == 0);
}

long	get_time_in_ms(void)
{
	struct timespec	ts;

	if (clock_gettime(CLOCK_MONOTONIC, &ts) == -1)
	{
		write(2, "clock_gettime failed", 20);
		return (-1);
	}
	return (ts.tv_sec * 1000 + ts.tv_nsec / 1000000);
}
