/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   monitor.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aarogarc <aarogarc@student.42malaga.c      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/13 17:47:44 by aarogarc          #+#    #+#             */
/*   Updated: 2026/04/13 17:47:53 by aarogarc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "codexion.h"

void	*monitor_all(void *arg)
{
	t_sim	*sim;
	int	i;

	sim = (t_sim *)arg;
	while (sim->stop != 1)
	{
		i = 0;
		while (i < sim->numbers_of_coders)
		{
			if ((calculate_ms() - sim->coder[i].last_compile_start) > sim->time_to_burnout)
			{
				print_log(&sim->coder[i], "burned out");
				sim->stop = 1;
			}
			if (sim->coder[i].n_compiles >= sim->compiles_required)
				sim->stop = 1;
			i++;
		}
		usleep(1000);
	}
	if (sim->stop == 1)
	{
		i = 0;
		while (i < sim->numbers_of_coders)
		{
			pthread_mutex_lock(&sim->dongles[i].mutex);
			pthread_cond_broadcast(&sim->dongles[i].cond);
			pthread_mutex_unlock(&sim->dongles[i].mutex);
			i++;
		}
	}
	return (NULL);
}