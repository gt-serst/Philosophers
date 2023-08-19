#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>
#include <unistd.h>

typedef struct s_main
{
	unsigned int	number_of_philosophers;
	unsigned int	time_to_die;
	unsigned int	time_to_eat;
	unsigned int	time_to_sleep;
	unsigned int	state[5];
	pthread_t		philosophers[5];
	pthread_mutex_t forks[5];
}	t_main;

typedef struct s_philo
{
	unsigned int	position;
	unsigned int	leftFork;
	unsigned int	rightFork;
}	t_philo;

void	init_struct(t_main *main, char **argv)
{
	main->number_of_philosophers = atoi(argv[1]);
	main->time_to_die = atoi(argv[2]);
	main->time_to_eat = atoi(argv[3]);
	main->time_to_sleep = atoi(argv[4]);
}

void	*philosophers(void *data)
{
	size_t	i;

	i = (size_t)data;
	printf("Welcome philosopher #%zu\n", i);
	return (NULL);
}

int	main(int argc, char **argv)
{
	size_t	i;
	t_main	main;

	(void)argc;
	init_struct(&main, argv);
	i = 0;
	while (i < main.number_of_philosophers)
	{
		pthread_create(&main.philosophers[i], NULL, philosophers, (void *)i);
		i++;
	}
	i = 0;
	while (i < main.number_of_philosophers)
	{
		pthread_join(main.philosophers[i], NULL);
		i++;
	}
	return (0);
}
