#include "codexion.h"

void    *coder_routine(void *arg) //pthread_create demands that the function must be declared like this
{
    t_coder *coder;

    coder = (t_coder *)arg; //cast to fix the parameter that the function recieves
    while (coder->sim->stop != 1)
    {
        fix_order(coder, coder->sim);
        print_log(coder, "has taken a dongle");
        coder->last_compile_start = calculate_ms();
        print_log(coder, "is compiling");
        precise_sleep(coder->sim->time_to_compile);
        free_dongle(coder->left_dongle);
        free_dongle(coder->right_dongle);
        print_log(coder, "is debugging");
        precise_sleep(coder->sim->time_to_debug);
        print_log(coder, "is refactoring");
        precise_sleep(coder->sim->time_to_refactor);
        coder->n_compiles += 1;
    }
    return (NULL);
}
