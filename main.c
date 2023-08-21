#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/time.h>

#define SLEEPING 0
#define THINKING 1
#define EATING 2

typedef struct s_philo
{
	int				pos;
	int				rightFork;
	int				leftFork;
	struct s_env	*env;	
	pthread_t		tid;
}	t_philo;

typedef struct s_env
{
	int				number_of_philosophers;
	int				time_to_die;
	int				time_to_eat;
	int				time_to_sleep;
	int				eat_count_max;
	long 			time;
	int				state[100];
	t_philo			philos[100];
	pthread_mutex_t forks[100];
	pthread_mutex_t state_change[100];
	pthread_mutex_t writing;
}	t_env;

void	is_eating(t_philo *philo)
{
	pthread_mutex_lock(&philo->env->writing);
	printf("Philosopher %d is eating\n", philo->pos + 1);
	pthread_mutex_unlock(&philo->env->writing);
}

void	is_sleeping(t_philo *philo)
{
	pthread_mutex_lock(&philo->env->writing);
	printf("Philosopher %d is sleeping\n", philo->pos + 1);
	pthread_mutex_unlock(&philo->env->writing);
}

void	take_forks(t_philo *philo)
{
	pthread_mutex_lock(&philo->env->forks[(philo->pos)]);
	pthread_mutex_lock(&philo->env->writing);
	printf("Philosopher %d has taken a fork\n", philo->pos + 1);
	pthread_mutex_unlock(&philo->env->writing);
	pthread_mutex_lock(&philo->env->forks[(philo->pos + 1) % philo->env->number_of_philosophers]);
	pthread_mutex_lock(&philo->env->writing);
	printf("Philosopher %d has taken a fork\n", philo->pos + 1);
	pthread_mutex_unlock(&philo->env->writing);
}

void	*philosophers(void *data)
{
	t_philo	*philo;

	philo = (t_philo *)data;
	pthread_mutex_lock(&philo->env->writing);
	printf("Philosopher %d has entered the room\n", philo->pos + 1);
	pthread_mutex_unlock(&philo->env->writing);
	while (1)
	{
		take_forks(philo);
		is_eating(philo);
		usleep(1000);
		pthread_mutex_lock(&philo->env->writing);
		printf("Philosopher %d has finished eating\n", philo->pos + 1);
		pthread_mutex_unlock(&philo->env->writing);
		pthread_mutex_unlock(&philo->env->forks[philo->pos]);
		pthread_mutex_unlock(&philo->env->forks[(philo->pos + 1) % philo->env->number_of_philosophers]);
		is_sleeping(philo);
		usleep(1000);
		pthread_mutex_lock(&philo->env->writing);
		printf("Philosopher %d has finished sleeping\n", philo->pos + 1);
		pthread_mutex_unlock(&philo->env->writing);
		printf("Il est actuellement: %d\n", gettimeofday(NULL, NULL));
	}
	return (NULL);
}

void	create_threads(t_env *env)
{
	int			i;

	i = 0;
	while (i < env->number_of_philosophers)
	{
		pthread_create(&env->philos[i].tid, NULL, philosophers, &(env->philos[i]));
		i++;
	}
}

void	join_threads(t_env *env)
{
	int	i;

	i = 0;
	while (i < env->number_of_philosophers)
	{
		pthread_join(env->philos[i].tid, NULL);
		i++;
	}
}

void	init_philos(t_env *env)
{
	int	i;

	i = 0;
	while (i < env->number_of_philosophers)
	{
		env->philos[i].pos = i;
		env->philos[i].rightFork = (i + 1) % env->number_of_philosophers;
		env->philos[i].leftFork = i;
		env->philos[i].env = env;
		i++;
	}
}

void	init_mutexes(t_env *env)
{
	int	i;

	i = 0;
	while (i < env->number_of_philosophers)
	{
		pthread_mutex_init(&env->forks[i], NULL);
		pthread_mutex_init(&env->state_change[i], NULL);
		i++;
	}
	pthread_mutex_init(&env->writing, NULL);
}

void	init_struct(t_env *env, char **argv)
{
	env->number_of_philosophers = atoi(argv[1]);
	env->time_to_die = atoi(argv[2]);
	env->time_to_eat = atoi(argv[3]);
	env->time_to_sleep = atoi(argv[4]);
	init_philos(env);
	init_mutexes(env);
}

void	destroy_mutexes(t_env *env)
{
	int	i;

	i = 0;
	while (i < env->number_of_philosophers)
	{
		pthread_mutex_destroy(&env->forks[i]);
		pthread_mutex_destroy(&env->state_change[i]);
		i++;
	}
	pthread_mutex_destroy(&env->writing);
}

int	main(int argc, char **argv)
{
	t_env	env;
	struct timeval current_time;

	gettimeofday(&current_time, NULL);
	env.time = current_time.tv_sec * 1000 + current_time.tv_usec / 1000;
	//printf("seconds : %ld\nmicro seconds : %d",current_time.tv_sec, current_time.tv_usec);
  	printf("milliseconds : %ld\n", env.time);
	(void)argc;
	init_struct(&env, argv);
	//create_threads(&env);
	//join_threads(&env);
	destroy_mutexes(&env);
	return (0);
}
