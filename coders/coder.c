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

static int	do_cycle(t_coder *coder)
{
	fix_order(coder, coder->sim);
	if (coder->sim->stop)
		return (1);
	make_coder_work(coder);
	if (coder->sim->stop)
		return (1);
	print_log(coder, "is debugging");
	calculate_sleep(coder->sim->time_to_debug, coder->sim);
	if (coder->sim->stop)
		return (1);
	print_log(coder, "is refactoring");
	calculate_sleep(coder->sim->time_to_refactor, coder->sim);
	coder->n_compiles += 1;
	if (coder->n_compiles >= coder->sim->compiles_required)
		return (1);
	return (0);
}

void	*coder_routine(void *arg)
{
	t_coder		*coder;
	long long	delay;

	coder = (t_coder *)arg;
	delay = (long long)(coder->id - 1) * coder->sim->time_to_compile
		/ coder->sim->numbers_of_coders;
	calculate_sleep(delay, coder->sim);
	coder->last_compile_start = calculate_ms();
	while (coder->sim->stop != 1)
	{
		if (do_cycle(coder))
			break ;
	}
	return (NULL);
}
