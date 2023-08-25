/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gt-serst <gt-serst@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/23 14:59:44 by gt-serst          #+#    #+#             */
/*   Updated: 2023/08/25 16:46:10 by gt-serst         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILO_H
# define PHILO_H

# include <stdio.h>
# include <pthread.h>
# include <stdlib.h>
# include <unistd.h>
# include <sys/time.h>

typedef struct s_arg
{
	int				nb_phil;
	int				time_to_die;
	int				time_to_eat;
	int				time_to_sleep;
	int				nb_eat_max;
	int				init_time;
	int				stop_condition;
	pthread_mutex_t	*forks;
	pthread_mutex_t	writing;
	pthread_mutex_t	death;
}	t_arg;

typedef struct s_phil
{
	int				index;
	pthread_t		tid;
	long			last_meal;
	int				nb_eat;
	int				right_fork;
	int				left_fork;
	pthread_mutex_t	meal;
	t_arg			*arg;
}	t_phil;

typedef struct s_p
{
	t_phil	*ph;
	t_arg	a;
}	t_p;

int		ft_exit(char *message);
int		parsing(int argc, char **argv, t_p *p);
int		init_phil(t_p *p);
int		launch_threads(t_p *p);
int		monitor(t_p *p);
void	exit_threads(t_p *p);
int		death_checker(t_phil *ph);
int		ft_strlen(const char *s);
long	get_current_time(void);
void	ft_usleep_status(t_phil *ph, long int time_in_ms);
void	ft_usleep(long int time_in_ms);
void	print_status(char *message, t_phil *ph, int index);
void	print(char *message, t_phil *ph, int index);

#endif
