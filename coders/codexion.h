/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   codexion.h                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aarogarc <aarogarc@student.42malaga.c      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/05 11:12:59 by aarogarc          #+#    #+#             */
/*   Updated: 2026/05/05 11:13:01 by aarogarc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#ifndef CODEXION_H
# define CODEXION_H

# include <unistd.h>
# include <stdlib.h>
# include <stdio.h>
# include <string.h>
# include <pthread.h>
# include <sys/time.h>

typedef struct s_dongle		t_dongle;
typedef struct s_coder		t_coder;
typedef struct s_sim		t_sim;

typedef struct s_sim
{
	int				numbers_of_coders;
	long long		time_to_burnout;
	long long		time_to_compile;
	long long		time_to_debug;
	long long		time_to_refactor;
	int				compiles_required;
	long long		dongle_cooldown;
	int				scheduler;
	volatile int	stop;
	long long		start_time;
	pthread_mutex_t	log_mutex;
	t_dongle		*dongles;
	t_coder			*coder;
}	t_sim;

typedef struct s_dongle
{
	pthread_mutex_t	mutex;
	pthread_cond_t	cond;
	int				free;
	long long		released_ts;
	t_coder			**queue;
	int				queue_size;
}	t_dongle;

typedef struct s_coder
{
	int				id;
	pthread_t		thread;
	t_sim			*sim;
	long long		last_compile_start;
	volatile int	n_compiles;
	t_dongle		*left_dongle;
	t_dongle		*right_dongle;
}	t_coder;

int			parse_args(int argc, char **argv, t_sim *sim);
long long	calculate_ms(void);
long long	elapsed_ms(long long start_ms);
void		calculate_sleep(long long ms, t_sim *sim);
void		print_log(t_coder *coder, char *str);
void		dongle_order(t_dongle *dongle, t_sim *sim, t_coder *coder);
void		fix_order(t_coder *coder, t_sim *sim);
void		free_dongle(t_dongle *dongle);
void		*coder_routine(void *arg);
int			init_sim(t_sim *sim);
void		cleanup_sim(t_sim *sim);
void		*monitor_all(void *arg);
void		enqueue(t_dongle *dongle, t_coder *coder);
t_coder		*dequeue(t_dongle *dongle);

#endif
