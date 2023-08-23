/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gt-serst <gt-serst@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/21 22:34:21 by gt-serst          #+#    #+#             */
/*   Updated: 2023/08/23 14:52:28 by gt-serst         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/time.h>

typedef struct s_env
{
	int				index;
	int				num_philosophers;
	int				time_to_die;
	int				time_to_eat;
	int				time_to_sleep;
	int				eat_count_max;
	long 			init_time;
	long			last_meal;
	int				eat_count;
	int				never_ate;
	int				is_currently_eating;
	pthread_t		tid;
	pthread_mutex_t	*right_fork;
	pthread_mutex_t	*left_fork;
	pthread_mutex_t	writing;
	pthread_mutex_t id;
}	t_env;

typedef struct s_phil
{
	pthread_mutex_t	*forks;
	struct s_env	*env;
}	t_phil;

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

void	print(char *message, t_env *phil, int index)
{
	long	time;

	time = get_current_time();
	pthread_mutex_lock(&phil->writing);
	printf(message, time, index);
	pthread_mutex_unlock(&phil->writing);
}

void	is_eating(t_env *phil)
{
	pthread_mutex_lock(phil->left_fork);
	print("%ld %d has taken a fork\n", phil, phil->index);
	pthread_mutex_lock(phil->right_fork);
	print("%ld %d has taken a fork\n", phil, phil->index);
	//pthread_mutex_lock(&phil->id);
	phil->last_meal = get_current_time();
	phil->is_currently_eating = 1;
	//pthread_mutex_unlock(&phil->id);
	print("%ld %d is eating\n", phil, phil->index);
	ft_usleep(phil->time_to_eat);
	//pthread_mutex_lock(&phil->id);
	phil->is_currently_eating = 0;
	//pthread_mutex_unlock(&phil->id);
	if (phil->eat_count_max != -1)
	{
		pthread_mutex_lock(&phil->id);
		phil->eat_count++;
		pthread_mutex_unlock(&phil->id);
	}
	pthread_mutex_unlock(phil->left_fork);
	pthread_mutex_unlock(phil->right_fork);
}

void	is_sleeping(t_env *phil)
{
	print("%ld %d is sleeping\n", phil, phil->index);
	ft_usleep(phil->time_to_sleep);
}

void	*philosophers(void *data)
{
	t_env	*phil;

	phil = (t_env *)data;
	if (phil->index % 2 == 0)
		ft_usleep(phil->time_to_eat / 10);
	print("%ld %d is entered the room\n", phil, phil->index);
	phil->eat_count = 0;
	phil->never_ate = 0;
	phil->is_currently_eating = 0;
	if (get_current_time() - phil->init_time >= phil->time_to_die)
	{
		phil->never_ate = 1;
		return (NULL);
	}
	while (1)
	{
		is_eating(phil);
		is_sleeping(phil);
		print("%ld %d is thinking\n", phil, phil->index);
	}
	return (NULL);
}

int	create_threads(t_phil *phil)
{
	int		i;
	int		err;
	long	init_time;

	i = 0;
	init_time = get_current_time();
	while (i < phil->env->num_philosophers)
	{
		phil->env[i].index = i + 1;
		phil->env[i].init_time = init_time;
		phil->env[i].last_meal = get_current_time();
		err = pthread_create(&phil->env[i].tid, NULL, philosophers, &(phil->env[i]));
		if (err != 0)
			return (0);
		ft_usleep(10);
		i++;
	}
	return (1);
}

void	init_phil(t_phil *phil)
{
	int	i;

	i = 1;
	while (i < phil->env->num_philosophers)
	{
		phil->env[i].num_philosophers = phil->env->num_philosophers;
		phil->env[i].time_to_die = phil->env->time_to_die;
		phil->env[i].time_to_eat = phil->env->time_to_eat;
		phil->env[i].time_to_sleep = phil->env->time_to_sleep;
		phil->env[i].eat_count_max = phil->env->eat_count_max;
		i++;
	}
}

int	init_mutexes(t_phil *phil)
{
	int	i;

	if (pthread_mutex_init(&phil->env[0].writing, NULL) != 0)
		return (0);
	i = 1;
	while (i < phil->env->num_philosophers)
	{
		phil->env[i].writing = phil->env->writing;
	 	i++;
	 }
	i = 0;
	while (i < phil->env->num_philosophers)
	{
		if (pthread_mutex_init(&phil->forks[i], NULL) != 0)
			return (0);
		i++;
	}
	i = 0;
	while (i < phil->env->num_philosophers)
	{
		phil->env[i].left_fork = &phil->forks[i];
		phil->env[i].right_fork = &phil->forks[(i + 1) % phil->env->num_philosophers];
		i++;
	}
	return (1);
}

int	init_struct(t_phil *phil, char **argv)
{
	phil->env = (t_env *)malloc(sizeof(t_env) * atoi(argv[1]) + 1);
	if (!phil->env)
		return (0);
	phil->env[0].num_philosophers = atoi(argv[1]);
	phil->env[0].time_to_die = atoi(argv[2]);
	phil->env[0].time_to_eat = atoi(argv[3]);
	phil->env[0].time_to_sleep = atoi(argv[4]);
	if (phil->env[0].num_philosophers < 0 || phil->env[0].time_to_die < 0
			|| phil->env[0].time_to_eat < 0 || phil->env[0].time_to_sleep < 0)
		return (0);
	if (argv[5])
		phil->env[0].eat_count_max = atoi(argv[5]);
	else
		phil->env[0].eat_count_max = -1;
	init_phil(phil);
	phil->forks = (pthread_mutex_t *)malloc(sizeof(pthread_mutex_t) * atoi(argv[1]) + 1);
	if (!phil->forks)
		return (0);
	if (!init_mutexes(phil))
		return (0);
	return (1);
}

int	destroy_mutexes(t_phil *phil)
{
	int	i;

	pthread_mutex_destroy(&phil->env->writing);
	i = 0;
	while (i < phil->env->num_philosophers)
	{
		if(pthread_mutex_destroy(&phil->forks[i]) != 0)
			return (0);
		i++;
	}
	return (1);
}

int	everyone_is_full(t_phil *phil)
{
	int	i;
	int	count;

	i = 0;
	count = 0;
	while (i < phil->env->num_philosophers)
	{
		if (phil->env->eat_count_max != -1 && phil->env[i].eat_count >= phil->env->eat_count_max)
			count++;
		if (count == phil->env->num_philosophers)
			return (1);
		i++;
	}
	return (0);
}

void	free_all(t_phil *phil)
{
	if (phil->env)
		free(phil->env);
	if (phil->forks)
		free(phil->forks);
}

int	has_died(t_phil *phil)
{
	int	i;

	while (!everyone_is_full(phil))
	{
		i = 0;
		while (i < phil->env->num_philosophers)
		{
			if (get_current_time() - phil->env[i].last_meal >= phil->env->time_to_die)
			{
				print("%ld %d died\n", &phil->env[i], phil->env[i].index);
				return (1);
			}
			i++;
		}
	}
	return (0);
}

int	main(int argc, char **argv)
{
	t_phil	phil;

	if (argc == 5 || argc == 6)
	{
		if (!init_struct(&phil, argv))
		{
			free_all(&phil);
			return (EXIT_FAILURE);
		}
		if (!create_threads(&phil))
		{
			destroy_mutexes(&phil);
			free_all(&phil);
			return (EXIT_FAILURE);
		}
		if (has_died(&phil))
		{
			destroy_mutexes(&phil);
			free_all(&phil);
			return (EXIT_FAILURE);
		}
	}
	else
	{
		printf("Arguments error\n");
		return (EXIT_FAILURE);
	}
	destroy_mutexes(&phil);
	free_all(&phil);
	return (EXIT_SUCCESS);
}
