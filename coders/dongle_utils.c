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

static void	wait_cooldown(t_dongle *dongle, t_sim *sim)
{
	struct timeval	current;
	struct timespec	timespec;
	long long		remaining;

	remaining = sim->dongle_cooldown
		- (calculate_ms() - dongle->released_ts);
	gettimeofday(&current, NULL);
	timespec.tv_sec = current.tv_sec + remaining / 1000;
	timespec.tv_nsec = current.tv_usec * 1000
		+ (remaining % 1000) * 1000000;
	if (timespec.tv_nsec >= 1000000000)
	{
		timespec.tv_sec++;
		timespec.tv_nsec -= 1000000000;
	}
	pthread_cond_timedwait(&dongle->cond, &dongle->mutex, &timespec);
}

static void	make_dongle_wait(t_dongle *dongle, t_sim *sim, t_coder *coder)
{
	while (sim->stop == 0)
	{
		if (dongle->free == 0
			|| (dongle->queue_size > 0 && dongle->queue[0] != coder))
			pthread_cond_wait(&dongle->cond, &dongle->mutex);
		else if (calculate_ms() - dongle->released_ts < sim->dongle_cooldown)
			wait_cooldown(dongle, sim);
		else
			break ;
	}
}

void	dongle_order(t_dongle *dongle, t_sim *sim, t_coder *coder)
{
	pthread_mutex_lock(&dongle->mutex);
	enqueue(dongle, coder);
	make_dongle_wait(dongle, sim, coder);
	if (sim->stop == 0 && dongle->queue_size > 0)
	{
		dequeue(dongle);
		dongle->free = 0;
	}
	else if (sim->stop == 1)
		dequeue(dongle);
	pthread_mutex_unlock(&dongle->mutex);
}

static void	pick_dongle_order(t_coder *coder, t_dongle **first,
	t_dongle **second, t_sim *sim)
{
	int	left_index;
	int	right_index;

	left_index = coder->left_dongle - sim->dongles;
	right_index = coder->right_dongle - sim->dongles;
	if ((coder->id % 2 != 0 && left_index < right_index)
		|| (coder->id % 2 == 0 && left_index > right_index))
	{
		*first = coder->left_dongle;
		*second = coder->right_dongle;
	}
	else
	{
		*first = coder->right_dongle;
		*second = coder->left_dongle;
	}
}

void	fix_order(t_coder *coder, t_sim *sim)
{
	t_dongle	*first;
	t_dongle	*second;

	if (coder->left_dongle == coder->right_dongle)
	{
		dongle_order(coder->left_dongle, sim, coder);
		print_log(coder, "has taken a dongle");
		return ;
	}
	pick_dongle_order(coder, &first, &second, sim);
	dongle_order(first, sim, coder);
	print_log(coder, "has taken a dongle");
	dongle_order(second, sim, coder);
	print_log(coder, "has taken a dongle");
}
