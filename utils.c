/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gt-serst <gt-serst@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/23 16:27:09 by gt-serst          #+#    #+#             */
/*   Updated: 2023/08/23 18:35:48 by gt-serst         ###   ########.fr       */
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
	long			time;
	struct timeval	current_time;

	gettimeofday(&current_time, NULL);
	time = current_time.tv_sec * 1000 + current_time.tv_usec / 1000;
	return (time);
}

void	ft_usleep(long int time_in_ms)
{
	long int	start_time;

	start_time = 0;
	start_time = get_current_time();
	while ((get_current_time() - start_time) < time_in_ms)
		usleep(time_in_ms / 10);
}

void	print_status(char *message, t_phil *ph, int index)
{
	long	time;

	time = get_current_time();
	pthread_mutex_lock(&ph->arg->writing);
	printf(message, time, index);
	pthread_mutex_unlock(&ph->arg->writing);
}

int	ft_exit(char *message)
{
	printf("Error: %s", message);
	return (EXIT_FAILURE);
}
