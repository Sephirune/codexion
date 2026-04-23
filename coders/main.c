/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aarogarc <aarogarc@student.42malaga.c      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/23 11:35:08 by aarogarc          #+#    #+#             */
/*   Updated: 2026/04/23 11:35:20 by aarogarc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "codexion.h"

int	main(int argc, char **argv)
{
	t_sim		sim;
	pthread_t	thread;
	int			i;
	int			j;

	i = 0;
	j = 0;
	memset(&sim, 0, sizeof(t_sim));
	parse_args(argc, argv, &sim);
	init_sim(&sim);
	pthread_create(&thread, NULL, monitor_all, &sim);
	while (i < sim.numbers_of_coders)
	{
		pthread_create(&sim.coder[i].thread, NULL, coder_routine, &sim.coder[i]);
		i++;
	}
	pthread_join(thread, NULL);
	while (j < sim.numbers_of_coders)
	{
		pthread_join(sim.coder[j].thread, NULL);
		j++;
	}
	return (0);
}
