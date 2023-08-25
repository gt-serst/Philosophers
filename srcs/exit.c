/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gt-serst <gt-serst@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/25 11:39:16 by gt-serst          #+#    #+#             */
/*   Updated: 2023/08/25 16:43:38 by gt-serst         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

static int	monitor_eat(t_phil *ph)
{
	if (ph->arg->nb_eat_max != -1 && ph->nb_eat >= ph->arg->nb_eat_max)
	{
		pthread_mutex_lock(&ph->arg->death);
		ph->arg->stop_condition = 1;
		pthread_mutex_unlock(&ph->arg->death);
		return (1);
	}
	return (0);
}

static int	monitor_death(t_phil *ph)
{
	if (get_current_time() - ph->last_meal >= ph->arg->time_to_die)
	{
		pthread_mutex_lock(&ph->arg->death);
		ph->arg->stop_condition = 1;
		pthread_mutex_unlock(&ph->arg->death);
		print("%ld %d died\n", ph, ph->index);
		return (1);
	}
	return (0);
}

int	monitor(t_p *p)
{
	int	i;

	while (1)
	{
		i = 0;
		while (i < p->a.nb_phil)
		{
			pthread_mutex_lock(&p->ph[i].meal);
			if (monitor_death(&p->ph[i]) || monitor_eat(&p->ph[i]))
			{
				pthread_mutex_unlock(&p->ph[i].meal);
				return (0);
			}
			pthread_mutex_unlock(&p->ph[i].meal);
			i++;
		}
		usleep(100);
	}
	return (1);
}

void	exit_threads(t_p *p)
{
	int	i;

	if (p->a.nb_phil == 1)
		pthread_detach(p->ph[0].tid);
	else
	{
		i = -1;
		while (++i < p->a.nb_phil)
			pthread_join(p->ph[i].tid, NULL);
	}
	pthread_mutex_destroy(&p->a.writing);
	pthread_mutex_destroy(&p->a.death);
	i = -1;
	while (++i < p->a.nb_phil)
	{
		pthread_mutex_destroy(&p->a.forks[i]);
		pthread_mutex_destroy(&p->ph[i].meal);
	}
	free(p->ph);
	free(p->a.forks);
}
