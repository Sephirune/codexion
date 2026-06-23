/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   queue.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aarogarc <aarogarc@student.42malaga.c      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/05 11:21:23 by aarogarc          #+#    #+#             */
/*   Updated: 2026/05/05 11:21:25 by aarogarc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "codexion.h"

static int	is_earlier_deadline(t_coder *a, t_coder *b)
{
	long long	deadline1;
	long long	deadline2;

	deadline1 = a->last_compile_start + a->sim->time_to_burnout;
	deadline2 = b->last_compile_start + b->sim->time_to_burnout;
	if (deadline1 < deadline2)
		return (1);
	if (deadline1 > deadline2)
		return (0);
	return (a->id < b->id);
}

static void	insert_sorted(t_dongle *dongle, t_coder *coder, int position)
{
	int	i;

	i = dongle->queue_size;
	while (i > position)
	{
		dongle->queue[i] = dongle->queue[i - 1];
		i--;
	}
	dongle->queue[position] = coder;
	dongle->queue_size++;
}

void	enqueue(t_dongle *dongle, t_coder *coder)
{
	int	position;

	if (coder->sim->scheduler == 0)
	{
		dongle->queue[dongle->queue_size] = coder;
		dongle->queue_size++;
		return ;
	}
	position = 0;
	while (position < dongle->queue_size
		&& !is_earlier_deadline(coder, dongle->queue[position]))
		position++;
	insert_sorted(dongle, coder, position);
}

t_coder	*dequeue(t_dongle *dongle)
{
	t_coder	*first;
	int		i;

	if (dongle->queue_size == 0)
		return (NULL);
	first = dongle->queue[0];
	i = 0;
	while (i < dongle->queue_size - 1)
	{
		dongle->queue[i] = dongle->queue[i + 1];
		i++;
	}
	dongle->queue_size--;
	return (first);
}
