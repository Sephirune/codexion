
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
    pthread_mutex_t log_mutex;
}   t_sim;

int parse_args(int argc, char **argv, t_sim *sim);

#endif;