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

long long	calculate_ms(void)
{
	struct timeval	tv;
	long long		seconds;
	long long		useconds;

	gettimeofday(&tv, NULL);
	seconds = tv.tv_sec * 1000;
	useconds = tv.tv_usec / 1000;
	return (seconds + useconds);
}

long long	elapsed_ms(long long start_ms)
{
	return (calculate_ms() - start_ms);
}

void	calculate_sleep(long long ms, t_sim *sim)
{
	long long	start;
	long long	current;

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
	if (coder->sim->stop == 1)
	{
		pthread_mutex_unlock(&coder->sim->log_mutex);
		return ;
	}
	ts = calculate_ms() - coder->sim->start_time;
	printf("%lld %d %s\n", ts, coder->id, str);
	pthread_mutex_unlock(&coder->sim->log_mutex);
}

void	free_dongle(t_dongle *dongle)
{
	pthread_mutex_lock(&dongle->mutex);
	dongle->free = 1;
	dongle->released_ts = calculate_ms();
	pthread_cond_broadcast(&dongle->cond);
	pthread_mutex_unlock(&dongle->mutex);
}
