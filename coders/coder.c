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

void    *coder_routine(void *arg)
{
    t_coder *coder;

    coder = (t_coder *)arg;
    calculate_sleep((coder->id - 1) * 50, coder->sim);
    while (coder->sim->stop != 1)
    {
        fix_order(coder, coder->sim);
        if (coder->sim->stop)
            break ;
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
