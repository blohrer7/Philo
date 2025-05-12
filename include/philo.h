/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: blohrer <blohrer@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/06 15:37:45 by blohrer           #+#    #+#             */
/*   Updated: 2025/05/09 10:35:08 by blohrer          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILO_H
# define PHILO_H

# include <limits.h>
# include <pthread.h>
# include <stdbool.h>
# include <stdio.h>
# include <stdlib.h>
# include <string.h>
# include <sys/time.h>
# include <sys/types.h>
# include <unistd.h>
#include <semaphore.h>


typedef struct s_philo
{
	int				id;
	pthread_mutex_t	*left_fork;
	pthread_mutex_t	*right_fork;
	long			last_meal_time;
	int				meals_eaten;

	pthread_t		thread;
	struct s_data	*data;
}					t_philo;

typedef struct s_data
{
	int				nb_philo;
	int				time_to_die;
	int				time_to_eat;
	int				time_to_sleep;
	long			start_time;
	int				someone_died;
	int				must_eat;
	int				philos_finished;
	pthread_mutex_t	*forks;
	pthread_mutex_t	print_lock;
	pthread_mutex_t	death_lock;

	t_philo			*philos;
}					t_data;

int	ft_atoi(const char *str);
long	get_time_in_ms(void);
int	init_data(t_data *data, int argc, char *argv[]);
int	init_mutexes(t_data *data);
int	check_input(int argc, char *argv[]);
int	init_philosophers(t_data *data);
int	start_philosophers(t_data *data);
void	*philo_routine(void *arg);
int	simulation_should_stop(t_philo *philo);
void take_forks(t_philo *philo);
int eat(t_philo *philo);
void	*monitor(void *arg);
void	cleanup(t_data *data);
void	ft_usleep(long ms);




#endif
