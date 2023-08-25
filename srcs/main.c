/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gt-serst <gt-serst@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/21 22:34:21 by gt-serst          #+#    #+#             */
/*   Updated: 2023/08/25 16:44:21 by gt-serst         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int	ft_strlen(const char *s)
{
	int	tmp;

	tmp = 0;
	while (s[tmp] != 0)
		tmp++;
	return (tmp);
}

int	ft_exit(char *message)
{
	printf("Error: %s", message);
	return (EXIT_FAILURE);
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
	return (EXIT_SUCCESS);
}
