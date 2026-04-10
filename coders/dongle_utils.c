#include "codexion.h"

void    dongle_order(t_dongle *dongle, t_sim *sim)
{
    pthread_mutex_lock(&dongle->mutex);
    while (dongle->free == 0 || calculate_ms() - dongle->released_ts < sim->dongle_cooldown)
        pthread_cond_wait(&dongle->cond, &dongle->mutex);
    dongle->free = 0;
    pthread_mutex_unlock(&dongle->mutex);
}

void    fix_order(t_coder *coder, t_sim *sim)
{
    if (coder->id == sim->numbers_of_coders)
    {
        dongle_order(coder->right_dongle, sim);
        print_log(coder, "has taken a dongle");
        dongle_order(coder->left_dongle, sim);
        print_log(coder, "has taken a dongle");
    }
    else
    {
        dongle_order(coder->left_dongle, sim);
        print_log(coder, "has taken a dongle");
        dongle_order(coder->right_dongle, sim);
        print_log(coder, "has taken a dongle");
    }
}

void    free_dongle(t_dongle *dongle)
{
    pthread_mutex_lock(&dongle->mutex);
    dongle->free = 1;
    dongle->released_ts = calculate_ms();
    pthread_cond_broadcast(&dongle->cond);
    pthread_mutex_unlock(&dongle->mutex);
}