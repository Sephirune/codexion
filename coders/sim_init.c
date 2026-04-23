/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sim_init.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aarogarc <aarogarc@student.42malaga.c      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/13 17:30:44 by aarogarc          #+#    #+#             */
/*   Updated: 2026/04/13 17:30:48 by aarogarc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "codexion.h"

static int	init_mutex(t_sim *sim)
{
	int i;

	i = 0;
	sim->stop = 0;
	sim->start_time = calculate_ms();
	if (pthread_mutex_init(&sim->log_mutex, NULL) != 0)
		return (1);
	sim->dongles = malloc(sim->numbers_of_coders * sizeof(t_dongle));
	if (!sim->dongles)
		return (1);
	while (i < sim->numbers_of_coders)
	{
		sim->dongles[i].free = 1;
		sim->dongles[i].released_ts = 0;
		if (pthread_mutex_init(&sim->dongles[i].mutex, NULL) != 0)
			return (1);
		if (pthread_cond_init(&sim->dongles[i].cond, NULL) != 0)
			return (1);
		i++;
	}
	return (0);
}

static int	aux_coders(t_sim *sim)
{
	int i;

	i = 0;
	sim->coder = malloc(sim->numbers_of_coders * sizeof(t_coder));
	if (!sim->coder)
		return (1);
	while (i < sim->numbers_of_coders)
	{
		sim->coder[i].id = i + 1;
		sim->coder[i].n_compiles = 0;
		sim->coder[i].last_compile_start = calculate_ms();
		sim->coder[i].sim = sim;
		sim->coder[i].left_dongle = &sim->dongles[i];
		if (i + 1 == sim->numbers_of_coders)
			sim->coder[i].right_dongle = &sim->dongles[0];
		else
			sim->coder[i].right_dongle = &sim->dongles[i + 1];
		i++;
	}
	return (0);
}

int	init_sim(t_sim *sim)
{
	if (init_mutex(sim) != 0)
		return (1);
	if (aux_coders(sim) != 0)
		return (1);
	return (0);
}