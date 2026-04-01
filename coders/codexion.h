
#ifndef CODEXION_H
# define CODEXION_H

# include <unistd.h>
# include <stdlib.h>
# include <stdio.h>
# include <string.h>
# include <pthread.h>
# include <sys/time.h>

typedef struct s_sim
{
    int             numbers_of_coders;
    long long       time_to_burnout;
    long long       time_to_compile;
    long long       time_to_debug;
    long long       time_to_refactor;
    int             compiles_required;
    long long       dongle_cooldown;
    int             scheduler; // para fifo (first in/out) es = 0, para edf (earlier deadline first) = 1
    int             stop;
    long long       start_time;
	pthread_mutex_t	log_mutex; // protege los prints
	t_dongle		*dongles;
	t_coder			*coder;
}   t_sim;

typedef struct	s_dongle
{
	pthread_mutex_t mutex;
	pthread_cond_t	cond;
	int				free; // checker para ver si está ocupado
	long long		released_ts; // cuándo fue liberado
}	t_dongle;

typedef struct s_coder
{
	int				id;
	pthread_t		thread;
	t_sim			*sim;
	long long		last_compile_start;
	int				n_compiles;
	t_dongle		*left_dongle;
	t_dongle		*right_dongle;
}	t_coder;


int	parse_args(int argc, char **argv, t_sim *sim);
void	precise_sleep(long long ms);

#endif