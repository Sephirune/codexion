/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aarogarc <aarogarc@student.42malaga.c      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/31 11:35:46 by aarogarc          #+#    #+#             */
/*   Updated: 2026/03/31 11:35:47 by aarogarc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "codexion.h"

long long	calculate_ms(void) // returns the current time in milliseconds
{
	struct timeval tv; // tv returns the ms since the epoch
	long long	seconds;
	long long useconds;

	gettimeofday(&tv, NULL); // timezone must be NULL because is obsolete
	seconds = tv.tv_sec * 1000;
	useconds = tv.tv_usec /1000;
	return (seconds + useconds);
}

long long	elapsed_ms(long long start_ms)
{
	return (calculate_ms() - start_ms);
}

void	calculate_sleep(long long ms, t_sim *sim) // pauses the thread to prevent burnouts and also checks if the simulation is stopped to avoid that the threads keep sleeping when the simulation is stopped
{
    long long   start;
    long long   current;

    start = calculate_ms();
    while (1)
    {
        current = calculate_ms();
        if (current - start >= ms)
            break ;
        if (sim->stop)
            return ;
        usleep(100);
    }
}

void	print_log(t_coder *coder, char *str)
{
	long long	ts;
	
	pthread_mutex_lock(&coder->sim->log_mutex);
	ts = calculate_ms() - coder->sim->start_time;
	if (coder->sim->stop == 1)
	{
		pthread_mutex_unlock(&coder->sim->log_mutex);
		return ;
	}
	printf("%lld %d %s\n", ts, coder->id, str);
	pthread_mutex_unlock(&coder->sim->log_mutex);
}
