/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gt-serst <gt-serst@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/23 16:27:09 by gt-serst          #+#    #+#             */
/*   Updated: 2023/08/24 20:50:55 by gt-serst         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int	ft_strlen(const char *s)
{
	int	tmp;

	tmp = 0;
	while (s[tmp] != 0)
		tmp++;
	return (tmp);
}

long	get_current_time(void)
{
	struct timeval	current_time;
	long			timestamp;

	gettimeofday(&current_time, NULL);
	timestamp = current_time.tv_sec * 1000 + current_time.tv_usec / 1000;
	return (timestamp);
}

void	ft_usleep(t_phil *ph, long int time_in_ms)
{
	long int	start_time;

	start_time = 0;
	start_time = get_current_time();
	while ((get_current_time() - start_time) < time_in_ms)
	{
		if (death_checker(ph))
			return ;
		usleep(100);
	}
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

void	print_death(char *message, t_phil *ph, int index)
{
	int	timestamp;

	pthread_mutex_lock(&ph->arg->writing);
	timestamp = get_current_time() - ph->arg->init_time;
	printf(message, timestamp, index);
	pthread_mutex_unlock(&ph->arg->writing);
}

int	ft_exit(char *message)
{
	printf("Error: %s", message);
	return (EXIT_FAILURE);
}
