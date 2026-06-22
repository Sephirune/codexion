/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   coder.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aarogarc <aarogarc@student.42malaga.c      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/13 17:31:25 by aarogarc          #+#    #+#             */
/*   Updated: 2026/04/13 17:31:28 by aarogarc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "codexion.h"

static void	make_coder_work(t_coder *coder)
{
	coder->last_compile_start = calculate_ms();
	print_log(coder, "is compiling");
	calculate_sleep(coder->sim->time_to_compile, coder->sim);
	if (coder->left_dongle == coder->right_dongle)
		free_dongle(coder->left_dongle);
	else
	{
		free_dongle(coder->left_dongle);
		free_dongle(coder->right_dongle);
	}
}

void	*coder_routine(void *arg)
{
	t_coder	*coder;
	long long	delay;

	coder = (t_coder *)arg;
	/* FIX BUG2: stagger based on compile time so coders don't all pile up
	   on the same dongle. Even IDs get an extra half-compile-time offset
	   which breaks the symmetry that caused coder 1 to monopolize dongle[0] */
	delay = (long long)(coder->id - 1) * coder->sim->time_to_compile
		/ coder->sim->numbers_of_coders;
	calculate_sleep(delay, coder->sim);
	/* FIX BUG1: reset burnout timer AFTER the initial delay so the timer
	   only starts counting when the coder is actually ready to work */
	coder->last_compile_start = calculate_ms();
	while (coder->sim->stop != 1)
	{
		fix_order(coder, coder->sim);
		if (coder->sim->stop)
			break ;
		make_coder_work(coder);
		if (coder->sim->stop)
			break ;
		print_log(coder, "is debugging");
		calculate_sleep(coder->sim->time_to_debug, coder->sim);
		if (coder->sim->stop)
			break ;
		print_log(coder, "is refactoring");
		calculate_sleep(coder->sim->time_to_refactor, coder->sim);
		coder->n_compiles += 1;
		if (coder->n_compiles >= coder->sim->compiles_required)
			break ;
	}
	return (NULL);
}
