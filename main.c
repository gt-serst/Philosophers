/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gt-serst <gt-serst@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/21 22:34:21 by gt-serst          #+#    #+#             */
/*   Updated: 2023/08/22 14:44:52 by gt-serst         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/time.h>

#define SLEEPING 0
#define THINKING 1
#define EATING 2

typedef struct s_phil
{
	int				pos;
	int				right_fork;
	int				left_fork;
	int				last_meal;
	pthread_t		tid;
	struct s_env	*env;
}	t_phil;

typedef struct s_env
{
	int				num_philosophers;
	int				time_to_die;
	int				time_to_eat;
	int				time_to_sleep;
	int				eat_count_max;
	long 			init_time;
	int				exit_status;
	int				states[1000];
	t_phil			phils[1000];
	pthread_mutex_t m_forks[1000];
	pthread_mutex_t m_states[1000];
	pthread_mutex_t m_writing;
	pthread_mutex_t	m_exit_status;
}	t_env;

void	is_eating(t_phil *phil)
{
	pthread_mutex_lock(&phil->env->m_writing);
	printf("Philosopher %d is eating\n", phil->pos + 1);
	pthread_mutex_unlock(&phil->env->m_writing);
}

void	is_sleeping(t_phil *phil)
{
	pthread_mutex_lock(&phil->env->m_writing);
	printf("Philosopher %d is sleeping\n", phil->pos + 1);
	pthread_mutex_unlock(&phil->env->m_writing);
}

void	take_forks(t_phil *phil)
{
	pthread_mutex_lock(&phil->env->m_forks[(phil->pos)]);
	pthread_mutex_lock(&phil->env->m_writing);
	printf("Philosopher %d has taken a fork\n", phil->pos + 1);
	pthread_mutex_unlock(&phil->env->m_writing);
	pthread_mutex_lock(&phil->env->m_forks[(phil->pos + 1) % phil->env->num_philosophers]);
	pthread_mutex_lock(&phil->env->m_writing);
	printf("Philosopher %d has taken a fork\n", phil->pos + 1);
	pthread_mutex_unlock(&phil->env->m_writing);
}

int	has_died(t_phil *phil)
{
	struct timeval current_time;

	gettimeofday(&current_time, NULL);
	phil->last_meal = (current_time.tv_sec * 1000 + current_time.tv_usec / 1000) - phil->env->init_time;
	//printf("Time since last meal %d\n", philo->last_meal);
	if (phil->last_meal >= 1000)
	{
		pthread_mutex_lock(&phil->env->m_writing);
		printf("Philosopher %d died\n", phil->pos);
		pthread_mutex_unlock(&phil->env->m_writing);
		return (1);
	}
	return (0);
}

void	*philosophers(void *data)
{
	t_phil	*phil;
	int		exit_local;

	phil = (t_phil *)data;
	pthread_mutex_lock(&phil->env->m_exit_status);
	exit_local = phil->env->exit_status;
	pthread_mutex_unlock(&phil->env->m_exit_status);
	if (exit_local != 0)
		return (NULL);
	if (has_died(phil) == 1)
	{
		pthread_mutex_lock(&phil->env->m_exit_status);
		phil->env->exit_status = 1;
		pthread_mutex_unlock(&phil->env->m_exit_status);
	}
	//pthread_mutex_lock(&phil->env->m_writing);
	//printf("Philosopher %d has entered the room\n", philo->pos + 1);
	//pthread_mutex_unlock(&phil->env->m_writing);
	while (1)
	{
		pthread_mutex_lock(&phil->env->m_exit_status);
		exit_local = phil->env->exit_status;
		pthread_mutex_unlock(&phil->env->m_exit_status);
		if (exit_local != 0)
			return (NULL);
		take_forks(phil);
		is_eating(phil);
		usleep(10000);
		//pthread_mutex_lock(&phil->env->m_writing);
		//printf("Philosopher %d has finished eating\n", philo->pos + 1);
		//pthread_mutex_unlock(&phil->env->m_writing);
		pthread_mutex_unlock(&phil->env->m_forks[phil->pos]);
		pthread_mutex_unlock(&phil->env->m_forks[(phil->pos + 1) % phil->env->num_philosophers]);
		is_sleeping(phil);
		usleep(10000);
		//pthread_mutex_lock(&phil->env->m_writing);
		//printf("Philosopher %d has finished sleeping\n", philo->pos + 1);
		//pthread_mutex_unlock(&phil->env->m_writing);
	}
}

int	create_threads(t_env *env)
{
	int	i;
	int	err;

	i = 0;
	while (i < env->num_philosophers)
	{
		err = pthread_create(&env->phils[i].tid, NULL, philosophers, &(env->phils[i]));
		if (err != 0)
			return (0);
		i++;
	}
	return (1);
}

int	join_threads(t_env *env)
{
	int	i;
	int	err;

	i = 0;
	while (i < env->num_philosophers)
	{
		err = pthread_join(env->phils[i].tid, NULL);
		if (err != 0)
			return (0);
		i++;
	}
	return (1);
}

void	init_phils(t_env *env)
{
	int	i;

	i = 0;
	while (i < env->num_philosophers)
	{
		env->phils[i].pos = i;
		env->phils[i].right_fork = (i + 1) % env->num_philosophers;
		env->phils[i].left_fork = i;
		env->phils[i].env = env;
		i++;
	}
}

void	init_mutexes(t_env *env)
{
	int	i;

	i = 0;
	while (i < env->num_philosophers)
	{
		pthread_mutex_init(&env->m_forks[i], NULL);
		pthread_mutex_init(&env->m_states[i], NULL);
		i++;
	}
	pthread_mutex_init(&env->m_writing, NULL);
	pthread_mutex_init(&env->m_exit_status, NULL);
}

void	init_struct(t_env *env, char **argv)
{
	env->num_philosophers = atoi(argv[1]);
	env->time_to_die = atoi(argv[2]);
	env->time_to_eat = atoi(argv[3]);
	env->time_to_sleep = atoi(argv[4]);
	env->exit_status = 0;
	init_phils(env);
	init_mutexes(env);
}

void	destroy_mutexes(t_env *env)
{
	int	i;

	i = 0;
	while (i < env->num_philosophers)
	{
		pthread_mutex_destroy(&env->m_forks[i]);
		pthread_mutex_destroy(&env->m_states[i]);
		i++;
	}
	pthread_mutex_destroy(&env->m_writing);
	pthread_mutex_destroy(&env->m_exit_status);
}

int	main(int argc, char **argv)
{
	t_env	env;
	struct timeval current_time;

	gettimeofday(&current_time, NULL);
	env.init_time = current_time.tv_sec * 1000 + current_time.tv_usec / 1000;
	//printf("seconds : %ld\nmicro seconds : %d",current_time.tv_sec, current_time.tv_usec);
  	//printf("milliseconds : %ld\n", env.time);
	(void)argc;
	init_struct(&env, argv);
	if (!create_threads(&env))
		return (EXIT_FAILURE);
	if (!join_threads(&env))
		return (EXIT_FAILURE);
	destroy_mutexes(&env);
	return (EXIT_SUCCESS);
}
