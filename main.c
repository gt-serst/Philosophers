#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>
#include <unistd.h>

typedef struct s_philo
{
	int				number_of_philosophers;
	int				state[5];
	pthread_t		tid[5];
	pthread_mutex_t	lock[5];
}	t_philo;

void	is_thinking(t_philo *philo, size_t i)
{
	philo->state[i] = 1;
	pthread_mutex_lock(&philo->lock[i]);
	printf("Philosopher %d is thinking\n", i);
	pthread_mutex_unlock(&philo->lock[i]);

}

void	take_forks(t_philo *philo, size_t i)
{
	if (philo->state[i] == 1 && philo->state[(i - 1 + philo->number_of_philosophers) % philo->number_of_philosophers] != 2 && philo->state[(i + 1) % philo->number_of_philosophers] != 2)
	{
		pthread_mutex_lock(&philo->lock[i]);
		philo->state[i] = 2;
		pthread_mutex_unlock(&philo->lock[i]);
	}
}

void	is_eating(t_philo *philo, size_t i)
{
	usleep(5);
	pthread_mutex_lock(&philo->lock[i]);
	printf("Philosopher %d is eating\n", i);
	pthread_mutex_unlock(&philo->lock[i]);
}

void	put_forks(t_philo *philo, size_t i)
{
	pthread_mutex_lock(&philo->lock[i]);
	philo->state[i] = 0;
	pthread_mutex_unlock(&philo->lock[i]);
}

void	is_sleeping(t_philo *philo, size_t i)
{
	usleep(5);
	pthread_mutex_lock(&philo->lock[i]);
	printf("Philosopher %d is sleeping\n", i);
	pthread_mutex_unlock(&philo->lock[i]);
}

void	*philosophers(void *data)
{
	size_t	i;
	t_philo	*philo;

	philo = (t_philo *)data;
	pthread_mutex_lock(&philo->lock[i + 1]);
	i = i + 1;
	printf("Welcome philosopher %d!\n", i);
	pthread_mutex_unlock(&philo->lock[i]);
	is_thinking(philo, i);
	take_forks(philo, i);
	is_eating(philo, i);
	put_forks(philo, i);
	is_sleeping(philo, i);
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

	philo.number_of_philosophers = atoi(argv[1]);
	init_mutexes(&philo);
	create_threads(&philo);
	join_threads(&philo);
	destroy_mutexes(&philo);
	return (0);
}
