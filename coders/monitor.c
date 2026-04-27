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

static int	all_coders_done(t_sim *sim)
{
	int	i;

	i = 0;
	while (i < sim->numbers_of_coders)
	{
		if (sim->coder[i].n_compiles < sim->compiles_required)
			return (0);
		i++;
	}
	return (1);
}

static void	check_coder_status(t_sim *sim, int i) // checks if the coders have reached burnout or if they've finished the number of compiles
{	
	while (sim->stop != 1)
	{
		i = 0;
		while (i < sim->numbers_of_coders) //
		{
			if (sim->coder[i].n_compiles < sim->compiles_required && (calculate_ms() - sim->coder[i].last_compile_start) > sim->time_to_burnout)
			{
				print_log(&sim->coder[i], "burned out");
				sim->stop = 1;
			}
			i++;
		}
		if (all_coders_done(sim))
			sim->stop = 1;
		usleep(1000);
	}
}

void	*monitor_all(void *arg)
{
	t_sim	*sim;
	int		i;

	sim = (t_sim *)arg;
	check_coder_status(sim, i);
	if (sim->stop == 1) // if the simulation is stopped, I need to wake up all the coders that are waiting for a dongle to let them finish
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
