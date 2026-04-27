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

static void make_dongle_wait(t_dongle *dongle, t_sim *sim)
{
    struct timeval  current;
    struct timespec timespec; // I need to convert the cooldown time to timespec to be able to use the timedwait function
    long long       remaining; // remaining time of the cooldown
    
    while (sim->stop == 0) 
        {
            if (dongle->free == 0
                || (dongle->queue_size > 0 && dongle->queue[0] != sim->coder))
                pthread_cond_wait(&dongle->cond, &dongle->mutex); // cond_wait makes the dongle wait until it's free and it's the coder's turn in the queue
            else if (calculate_ms() - dongle->released_ts < sim->dongle_cooldown)
            {
                remaining = sim->dongle_cooldown - (calculate_ms() - dongle->released_ts);
                gettimeofday(&current, NULL);
                timespec.tv_sec = current.tv_sec + remaining / 1000;
                timespec.tv_nsec = current.tv_usec * 1000 + (remaining % 1000) * 1000000; // convert the remaining time to seconds and nanoseconds
                if (timespec.tv_nsec >= 1000000000) // if the nanoseconds exceed 1 second, we need to convert it to seconds and nanoseconds
                {
                    timespec.tv_sec++;
                    timespec.tv_nsec -= 1000000000;
                }
                pthread_cond_timedwait(&dongle->cond, &dongle->mutex, &timespec); // if the dongle is in cd, it waits until the cd is over or until it's free and it's its turn
            }
            else
                break ;
        }
}

void    dongle_order(t_dongle *dongle, t_sim *sim, t_coder *coder)
{
    pthread_mutex_lock(&dongle->mutex);
    enqueue(dongle, coder);
    make_dongle_wait(dongle, sim);
    if (sim->stop == 0 && dongle->queue_size > 0) // if the simulation is not stopped and there are coders in the queue
    {
        dequeue(dongle);
        dongle->free = 0;
    }
    else if (sim->stop == 1) // if the simulation is stopped, I remove the coders from the queue
        dequeue(dongle);
    pthread_mutex_unlock(&dongle->mutex);
}

void    fix_order(t_coder *coder, t_sim *sim)
{
    t_dongle    *first;
    t_dongle    *second;
    int         left_index;
    int         right_index;

    left_index = coder->left_dongle - sim->dongles;
    right_index = coder->right_dongle - sim->dongles;
    
    if (coder->left_dongle == coder->right_dongle) // if there is only one dongle, the coder only need to take it once
    {
        dongle_order(coder->left_dongle, sim, coder);
        print_log(coder, "has taken a dongle");
        return ;
    }
    if (left_index < right_index) // to avoid deadlocks, the coder with the smaller index of dongle will try to take it first
    {
        first = coder->left_dongle;
        second = coder->right_dongle;
    }
    else
    {
        first = coder->right_dongle;
        second = coder->left_dongle;
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
