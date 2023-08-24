/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gt-serst <gt-serst@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/21 22:34:21 by gt-serst          #+#    #+#             */
/*   Updated: 2023/08/24 14:58:35 by gt-serst         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

static int	not_full(t_p *p)
{
	int	i;

	i = 0;
	while (i < p->a.nb_phil)
	{
		if (p->a.nb_eat_max == -1 || p->ph[i].nb_eat < p->a.nb_eat_max)
			return (1);
		i++;
	}
	p->a.all_eaten_ntimes = 1;
	return (0);
}

static int	has_died(t_p *p)
{
	int	i;

	while (not_full(p))
	{
		i = 0;
		pthread_mutex_lock(&p->a.end);
		while (i < p->a.nb_phil)
		{
			if (get_current_time() - p->ph[i].last_meal >= p->a.time_to_die)
			{
				print_status("%ld %d died\n", &p->ph[i], p->ph[i].index);
				return (1);
			}
			i++;
		}
		pthread_mutex_unlock(&p->a.end);
	}
	return (0);
}

static void	stop(t_p *p)
{
	int	i;

	has_died(p);
	pthread_mutex_destroy(&p->a.writing);
	pthread_mutex_destroy(&p->a.eating);
	pthread_mutex_destroy(&p->a.end);
	i = -1;
	while (++i < p->a.nb_phil)
		pthread_mutex_destroy(&p->a.forks[i]);
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
	return (EXIT_SUCCESS);
}
