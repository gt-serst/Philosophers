/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   launch.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gt-serst <gt-serst@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/23 16:25:15 by gt-serst          #+#    #+#             */
/*   Updated: 2023/08/23 19:39:27 by gt-serst         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

static void	is_eating(t_phil *ph)
{
	pthread_mutex_lock(ph->left_fork);
	print_status("%ld %d has taken a fork\n", ph, ph->index);
	pthread_mutex_lock(ph->right_fork);
	print_status("%ld %d has taken a fork\n", ph, ph->index);
	pthread_mutex_lock(&ph->arg->eating);
	ph->last_meal = get_current_time();
	pthread_mutex_unlock(&ph->arg->eating);
	ph->is_currently_eating = 1;
	print_status("%ld %d is eating\n", ph, ph->index);
	ft_usleep(ph->arg->time_to_eat);
	ph->is_currently_eating = 0;
	if (ph->arg->nb_eat_max != -1)
	{
		pthread_mutex_lock(&ph->arg->eating);
		ph->nb_eat++;
		pthread_mutex_unlock(&ph->arg->eating);
	}
	pthread_mutex_unlock(ph->left_fork);
	pthread_mutex_unlock(ph->right_fork);
}

static void	is_sleeping(t_phil *ph)
{
	print_status("%ld %d is sleeping\n", ph, ph->index);
	ft_usleep(ph->arg->time_to_sleep);
}

static void	*philosophers(void *data)
{
	t_phil	*ph;

	ph = (t_phil *)data;
	if (ph->index % 2 == 0)
		ft_usleep(ph->arg->time_to_eat / 10);
	if (get_current_time() - ph->arg->init_time >= ph->arg->time_to_die)
	{
		ph->never_ate = 1;
		return (NULL);
	}
	while (ph->arg->nb_eat_max == -1 || !ph->arg->all_eaten_ntimes)
	{
		is_eating(ph);
		is_sleeping(ph);
		print_status("%ld %d is thinking\n", ph, ph->index);
	}
	return (NULL);
}

int	launch_threads(t_p *p)
{
	int		i;

	i = 0;
	p->a.init_time = get_current_time();
	p->a.all_eaten_ntimes = 0;
	while (i < p->a.nb_phil)
	{
		p->ph[i].arg = &p->a;
		p->ph[i].last_meal = get_current_time();
		if (pthread_create(&p->ph[i].tid, NULL, philosophers, &(p->ph[i])) != 0)
			return(ft_exit("Thread not launch properly\n"));
		ft_usleep(10);
		i++;
	}
	return (1);
}
