/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gt-serst <gt-serst@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/23 16:27:09 by gt-serst          #+#    #+#             */
/*   Updated: 2023/08/25 16:44:17 by gt-serst         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

long	get_current_time(void)
{
	struct timeval	current_time;
	long			timestamp;

	gettimeofday(&current_time, NULL);
	timestamp = (current_time.tv_sec * 1000) + (current_time.tv_usec / 1000);
	return (timestamp);
}

void	ft_usleep_status(t_phil *ph, long int time_in_ms)
{
	long int	start_time;

	(void)ph;
	start_time = 0;
	start_time = get_current_time();
	while ((get_current_time() - start_time) < time_in_ms)
	{
		if (death_checker(ph))
			return ;
		usleep(100);
	}
}

void	ft_usleep(long int time_in_ms)
{
	long int	start_time;

	start_time = 0;
	start_time = get_current_time();
	while ((get_current_time() - start_time) < time_in_ms)
		usleep(100);
}

void	print_status(char *message, t_phil *ph, int index)
{
	int	timestamp;

	pthread_mutex_lock(&ph->arg->writing);
	timestamp = get_current_time() - ph->arg->init_time;
	if (!death_checker(ph))
		printf(message, timestamp, index);
	pthread_mutex_unlock(&ph->arg->writing);
}

void	print(char *message, t_phil *ph, int index)
{
	int	timestamp;

	pthread_mutex_lock(&ph->arg->writing);
	timestamp = get_current_time() - ph->arg->init_time;
	printf(message, timestamp, index);
	pthread_mutex_unlock(&ph->arg->writing);
}
