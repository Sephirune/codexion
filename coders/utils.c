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
	struct timeval tv; // tv returns the ms since the epoch
	long long	seconds;
	long long useconds;

	gettimeofday(&tv, NULL); // timezone must be NULL because is obsolete
	seconds = tv.tv_sec * 1000;
	useconds = tv.tv_usec /1000;
	return (seconds + useconds);
}

long long elapsed_ms(long long start_ms)
{
	return (calculate_ms() - start_ms);
}

void    calculate_sleep(long long ms) // pauses the thread to prevent burnouts
{
	long long	start;
	long long	current;
	
	start = calculate_ms();
	while (1)
	{
		current = get_time_ms();
		if (current - start >= ms)
			break;
		usleep(100);
	}
}