/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   dongle_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aarogarc <aarogarc@student.42malaga.c      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/13 17:31:02 by aarogarc          #+#    #+#             */
/*   Updated: 2026/04/13 17:31:06 by aarogarc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "codexion.h"

void	dongle_order(t_dongle *dongle, t_sim *sim, t_coder *coder)
{
	pthread_mutex_lock(&dongle->mutex);
	enqueue(dongle, coder);
	while ((dongle->free == 0 || calculate_ms() - dongle->released_ts < sim->dongle_cooldown || dongle->queue[0] != coder) && sim->stop == 0)
		pthread_cond_wait(&dongle->cond, &dongle->mutex);
	if (sim->stop == 0)
	{
        dongle->free = 0;
        dongle->free = 0;
    }
	pthread_mutex_unlock(&dongle->mutex);
}

void    fix_order(t_coder *coder, t_sim *sim)
{
    t_dongle    *first;
    t_dongle    *second;

    if (coder->id == sim->numbers_of_coders)
    {
        first = coder->right_dongle;
        second = coder->left_dongle;
    }
    else
    {
        first = coder->left_dongle;
        second = coder->right_dongle;
    }
    dongle_order(first, sim, coder);
    print_log(coder, "has taken a dongle");
    dongle_order(second, sim, coder);
    print_log(coder, "has taken a dongle");
}

void	free_dongle(t_dongle *dongle)
{
	pthread_mutex_lock(&dongle->mutex);
	dongle->free = 1;
	dongle->released_ts = calculate_ms();
	pthread_cond_broadcast(&dongle->cond);
	pthread_mutex_unlock(&dongle->mutex);
}
