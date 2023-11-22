/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   launch.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gt-serst <gt-serst@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/23 16:25:15 by gt-serst          #+#    #+#             */
/*   Updated: 2023/10/26 10:02:23 by gt-serst         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/philo.h"

static void	is_eating(t_phil *ph)
{
	pthread_mutex_lock(&ph->arg->forks[ph->left_fork]);
	print_status("%ld %d has taken a fork\n", ph, ph->index);
	pthread_mutex_lock(&ph->arg->forks[ph->right_fork]);
	print_status("%ld %d has taken a fork\n", ph, ph->index);
	pthread_mutex_lock(&ph->meal);
	ph->last_meal = get_current_time();
	ph->nb_eat++;
	pthread_mutex_unlock(&ph->meal);
	if (ph->nb_eat == ph->arg->nb_eat_max)
	{
		pthread_mutex_lock(&ph->arg->eat_count);
		ph->arg->eat_max++;
		pthread_mutex_unlock(&ph->arg->eat_count);
	}
	print_status("%ld %d is eating\n", ph, ph->index);
	ft_usleep_status(ph, ph->arg->time_to_eat);
	pthread_mutex_unlock(&ph->arg->forks[ph->left_fork]);
	pthread_mutex_unlock(&ph->arg->forks[ph->right_fork]);
}

int	death_checker(t_phil *ph)
{
	pthread_mutex_lock(&ph->arg->death);
	if (ph->arg->stop_condition == 1)
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
		ft_usleep(10);
	while (!death_checker(ph))
	{
		is_eating(ph);
		print_status("%ld %d is sleeping\n", ph, ph->index);
		ft_usleep_status(ph, ph->arg->time_to_sleep);
		print_status("%ld %d is thinking\n", ph, ph->index);
	}
	return (NULL);
}

int	launch_threads(t_p *p)
{
	int		i;

	i = 0;
	p->a.eat_max = 0;
	p->a.stop_condition = 0;
	p->a.init_time = get_current_time();
	while (i < p->a.nb_phil)
	{
		p->ph[i].arg = &p->a;
		p->ph[i].last_meal = get_current_time();
		if (pthread_create(&p->ph[i].tid, NULL, philosophers, &(p->ph[i])) != 0)
			return (ft_exit("Thread not launch properly\n"));
		i++;
	}
	monitor(p);
	exit_threads(p);
	return (1);
}
