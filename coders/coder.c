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

void	*coder_routine(void *arg) //pthread_create demands that the function must be declared like this
{
	t_coder *coder;

	coder = (t_coder *)arg; //cast to fix the parameter that the function recieves
	while (coder->sim->stop != 1)
	{
		fix_order(coder, coder->sim);
		coder->last_compile_start = calculate_ms();
		print_log(coder, "is compiling");
		calculate_sleep(coder->sim->time_to_compile);
		free_dongle(coder->left_dongle);
		free_dongle(coder->right_dongle);
		print_log(coder, "is debugging");
		calculate_sleep(coder->sim->time_to_debug);
		print_log(coder, "is refactoring");
		calculate_sleep(coder->sim->time_to_refactor);
		coder->n_compiles += 1;
	}
	return (NULL);
}
