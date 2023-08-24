/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   launch.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gt-serst <gt-serst@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/23 16:25:15 by gt-serst          #+#    #+#             */
/*   Updated: 2023/08/24 20:35:20 by gt-serst         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

static void	is_eating(t_phil *ph)
{
	pthread_mutex_lock(ph->left_fork);
	print_status("%ld %d has taken a fork\n", ph, ph->index);
	if (ph->left_fork == ph->right_fork)
	{
		ph->arg->is_dead = 1;
		return ;
	}
	pthread_mutex_lock(ph->right_fork);
	print_status("%ld %d has taken a fork\n", ph, ph->index);
	pthread_mutex_lock(&ph->arg->death);
	ph->last_meal = get_current_time();
	pthread_mutex_unlock(&ph->arg->death);
	print_status("%ld %d is eating\n", ph, ph->index);
	ft_usleep(ph, ph->arg->time_to_eat);
	if (ph->arg->nb_eat_max != -1)
	{
		pthread_mutex_lock(&ph->arg->eat);
		ph->nb_eat++;
		pthread_mutex_unlock(&ph->arg->eat);
	}
	pthread_mutex_unlock(ph->left_fork);
	pthread_mutex_unlock(ph->right_fork);
}

int	death_checker(t_phil *ph)
{
	pthread_mutex_lock(&ph->arg->death);
	if (ph->arg->is_dead == 1)
	{
		pthread_mutex_unlock(&ph->arg->death);
		return (1);
	}
	pthread_mutex_unlock(&ph->arg->death);
	return (0);
}

static void	*philosophers(void *data)
{
	t_phil	*ph;

	ph = (t_phil *)data;
	if (ph->index % 2 == 0)
		ft_usleep(ph, ph->arg->time_to_eat / 10);
	while (!death_checker(ph))
	{
		is_eating(ph);
		print_status("%ld %d is sleeping\n", ph, ph->index);
		ft_usleep(ph, ph->arg->time_to_sleep);
		print_status("%ld %d is thinking\n", ph, ph->index);
		pthread_mutex_lock(&ph->arg->eat);
		if (ph->arg->all_eaten == 1)
		{
			pthread_mutex_unlock(&ph->arg->eat);
			break ;
		}
		pthread_mutex_unlock(&ph->arg->eat);
	}
	return (NULL);
}

int	launch_threads(t_p *p)
{
	int		i;

	i = 0;
	p->a.init_time = get_current_time();
	p->a.is_dead = 0;
	p->a.all_eaten = 0;
	while (i < p->a.nb_phil)
	{
		p->ph[i].arg = &p->a;
		p->ph[i].last_meal = get_current_time();
		if (pthread_create(&p->ph[i].tid, NULL, philosophers, &(p->ph[i])) != 0)
			return(ft_exit("Thread not launch properly\n"));
		i++;
	}
	return (1);
}
