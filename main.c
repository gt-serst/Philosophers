/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gt-serst <gt-serst@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/21 22:34:21 by gt-serst          #+#    #+#             */
/*   Updated: 2023/08/24 20:06:43 by gt-serst         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

static int	not_full(t_p *p)
{
	int	i;

	i = 0;
	pthread_mutex_lock(&p->a.eat);
	while (i < p->a.nb_phil)
	{
		if (p->a.nb_eat_max == -1 || p->ph[i].nb_eat < p->a.nb_eat_max)
		{
			pthread_mutex_unlock(&p->a.eat);
			return (1);
		}
		i++;
	}
	p->a.all_eaten = 1;
	pthread_mutex_unlock(&p->a.eat);
	return (0);
}

static int	death_monitor(t_p *p)
{
	int	i;

	while (not_full(p))
	{
		i = 0;
		while (i < p->a.nb_phil)
		{
			pthread_mutex_lock(&p->a.death);
			if (get_current_time() - p->ph[i].last_meal >= p->a.time_to_die)
			{
				p->a.is_dead = 1;
				pthread_mutex_unlock(&p->a.death);
				print_death("%ld %d died\n", &p->ph[i], p->ph[i].index);
				return (1);
			}
			pthread_mutex_unlock(&p->a.death);
			i++;
		}
	}
	return (0);
}

static void	stop(t_p *p)
{
	int	i;

	death_monitor(p);
	i = 0;
	while (i < p->a.nb_phil)
	{
		pthread_join(p->ph[i].tid, NULL);
		i++;
	}
	pthread_mutex_destroy(&p->a.writing);
	pthread_mutex_destroy(&p->a.eat);
	pthread_mutex_destroy(&p->a.death);
	i = 0;
	while (i < p->a.nb_phil)
	{
		pthread_mutex_destroy(&p->a.forks[i]);
		i++;
	}
	free(p->ph);
	free(p->a.forks);
}

int	main(int argc, char **argv)
{
	t_p	p;

	if (!parsing(argc, argv, &p))
		return (ft_exit("Invalid arguments\n"));
	p.ph = (t_phil *)malloc(sizeof(t_phil) * p.a.nb_phil + 1);
	if (!p.ph)
		return (EXIT_FAILURE);
	if (!init_phil(&p) || !launch_threads(&p))
	{
		free(p.ph);
		free(p.a.forks);
		return (EXIT_FAILURE);
	}
	stop(&p);
	return (EXIT_FAILURE);
/*
	if (death_monitor(&p))
		return (EXIT_FAILURE);
	else
		return (EXIT_SUCCESS);
*/
}
