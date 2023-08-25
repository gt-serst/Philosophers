/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gt-serst <gt-serst@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/23 14:59:15 by gt-serst          #+#    #+#             */
/*   Updated: 2023/08/25 16:45:18 by gt-serst         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

static int	ft_atoi(const char *str)
{
	int		tmp;
	int		sign;
	int		number;

	tmp = 0;
	sign = 1;
	number = 0;
	while (str[tmp] == 32 || (str[tmp] >= 9 && str[tmp] <= 13))
		tmp++;
	if (str[tmp] == '-')
	{
		sign = -sign;
		tmp++;
	}
	else if (str[tmp] == '+')
		tmp++;
	while (str[tmp] >= '0' && str[tmp] <= '9')
	{
		number = number * 10 + str[tmp] - '0';
		tmp++;
	}
	return (number * sign);
}

static int	check_argv(int argc, char **argv)
{
	int	i;
	int	j;

	if (argc != 5 && argc != 6)
		return (0);
	i = 1;
	while (argv[i])
	{
		j = 0;
		while (argv[i][j])
		{
			if (argv[i][j] < '0' || argv[i][j] > '9' || ft_strlen(argv[i]) > 10)
				return (0);
			j++;
		}
		i++;
	}
	return (1);
}

int	parsing(int argc, char **argv, t_p *p)
{
	if (check_argv(argc, argv))
	{
		p->a.nb_phil = ft_atoi(argv[1]);
		p->a.time_to_die = ft_atoi(argv[2]);
		p->a.time_to_eat = ft_atoi(argv[3]);
		p->a.time_to_sleep = ft_atoi(argv[4]);
		if (argc == 6)
			p->a.nb_eat_max = atoi(argv[5]);
		else
			p->a.nb_eat_max = -1;
		if (p->a.nb_phil <= 0 || p->a.time_to_die <= 0
			|| p->a.time_to_eat <= 0 || p->a.time_to_sleep <= 0)
			return (0);
		else
			return (1);
	}
	else
		return (0);
}

static int	init_mutexes(t_p *p)
{
	int	i;

	if (pthread_mutex_init(&p->a.writing, NULL) != 0)
		return (0);
	if (pthread_mutex_init(&p->a.death, NULL) != 0)
		return (0);
	i = 0;
	while (i < p->a.nb_phil)
	{
		if (pthread_mutex_init(&p->a.forks[i], NULL) != 0)
			return (0);
		if (pthread_mutex_init(&p->ph[i].meal, NULL) != 0)
			return (0);
		i++;
	}
	return (1);
}

int	init_phil(t_p *p)
{
	int	i;

	i = 0;
	while (i < p->a.nb_phil)
	{
		p->ph[i].index = i + 1;
		p->ph[i].left_fork = i;
		p->ph[i].right_fork = (i + 1) % p->a.nb_phil;
		p->ph[i].nb_eat = 0;
		i++;
	}
	p->a.forks = (pthread_mutex_t *)malloc
		(sizeof(pthread_mutex_t) * p->a.nb_phil + 1);
	if (!p->a.forks)
		return (0);
	if (!init_mutexes(p))
		return (0);
	return (1);
}
