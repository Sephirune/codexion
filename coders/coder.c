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

static void make_coder_work(t_coder *coder) // makes the coder compile and free the dongles after compiling
{
        coder->last_compile_start = calculate_ms();
        print_log(coder, "is compiling");
        calculate_sleep(coder->sim->time_to_compile, coder->sim);
        if (coder->left_dongle == coder->right_dongle) // if there is only one dongle, the coder need to free it twice to be able to compile again
            free_dongle(coder->left_dongle);
        else
        {
            free_dongle(coder->left_dongle);
            free_dongle(coder->right_dongle);
        }
}

void    *coder_routine(void *arg)
{
    t_coder *coder;

    coder = (t_coder *)arg;
    calculate_sleep((coder->id - 1) * 50, coder->sim); // delay to avoid that all the coders try to compile at the same time at the beginning
    while (coder->sim->stop != 1) // the coder keeps compiling, debugging and refactoring until it reaches the required number of compiles or until it reaches burnout
    {
        fix_order(coder, coder->sim);
        if (coder->sim->stop)
            break ;
        make_coder_work(coder);
        if (coder->sim->stop) // kills the simulation if the coder reached burnout while compiling
            break ;
        print_log(coder, "is debugging");
        calculate_sleep(coder->sim->time_to_debug, coder->sim);
        if (coder->sim->stop)
            break ;
        print_log(coder, "is refactoring");
        calculate_sleep(coder->sim->time_to_refactor, coder->sim);
        coder->n_compiles += 1;
        if (coder->n_compiles >= coder->sim->compiles_required) // if the coder meets the required number of compiles, it stops
            break ;
    }
    return (NULL);
}
