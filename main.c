#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>
#include <unistd.h>


typedef struct s_philo
{
	int		n;
	int		number_of_philosophers;
	int		state[5];
	pthread_t	tid[5];
	pthread_mutex_t	lock[5];
}	t_philo;

void	is_thinking(t_philo *philo)
{
	philo->state[philo->n] = 1;
	pthread_mutex_lock(&philo->lock[philo->n]);
	printf("Philosopher %d is thinking\n", philo->n);
	pthread_mutex_unlock(&philo->lock[philo->n]);
	
}

void	take_forks(t_philo *philo)
{
	if (philo->state[philo->n] == 1 && philo->state[(philo->n - 1 + philo->number_of_philosophers) % philo->number_of_philosophers] != 2 && philo->state[(philo->n + 1) % philo->number_of_philosophers] != 2)
	{
		pthread_mutex_lock(&philo->lock[philo->n]);
		philo->state[philo->n] = 2;
		pthread_mutex_unlock(&philo->lock[philo->n]);
	}
}

void	is_eating(t_philo *philo)
{
	usleep(5);
	pthread_mutex_lock(&philo->lock[philo->n]);
	printf("Philosopher %d is eating\n", philo->n);
	pthread_mutex_unlock(&philo->lock[philo->n]);
}

void	put_forks(t_philo *philo)
{
	pthread_mutex_lock(&philo->lock[philo->n]);
	philo->state[philo->n] = 0;
	pthread_mutex_unlock(&philo->lock[philo->n]);
}

void	is_sleeping(t_philo *philo)
{
	usleep(5);
	pthread_mutex_lock(&philo->lock[philo->n]);
	printf("Philosopher %d is sleeping\n", philo->n);
	pthread_mutex_unlock(&philo->lock[philo->n]);	
}

void	*philosophers(void *data)
{
	t_philo	*philo;

	philo = (t_philo *)data;
	pthread_mutex_lock(&philo->lock[philo->n + 1]);
	philo->n = philo->n + 1;
	printf("Welcome philosopher %d!\n", philo->n);
	pthread_mutex_unlock(&philo->lock[philo->n]);
	is_thinking(philo);
	take_forks(philo);
	is_eating(philo);
	put_forks(philo);
	is_sleeping(philo);
	return (NULL);
}

void	init_mutexes(t_philo *philo)
{
	int	i;

	i = 0;
	while (i < philo->number_of_philosophers)
	{
		pthread_mutex_init(&philo->lock[i], NULL);
		i++;
	}
}

void	create_threads(t_philo *philo)
{
	int	i;

	i = 0;
	while (i < philo->number_of_philosophers)
	{
		pthread_create(&philo->tid[i], NULL, philosophers, philo);
		i++;
	}
}

void	join_threads(t_philo *philo)
{
	int	i;

	i = 0;
	while (i < philo->number_of_philosophers)
	{
		pthread_join(philo->tid[i], NULL);
		i++;
	}
}

void	destroy_mutexes(t_philo *philo)
{
	int	i;

	i = 0;
	while (i < philo->number_of_philosophers)
	{
		pthread_mutex_destroy(&philo->lock[i]);
		i++;
	}
}

int	main(int argc, char **argv)
{
	t_philo	philo;

	philo.n = -1;
	philo.number_of_philosophers = atoi(argv[1]);
	init_mutexes(&philo);
	create_threads(&philo);
	join_threads(&philo);
	destroy_mutexes(&philo);
	return (0);
}
