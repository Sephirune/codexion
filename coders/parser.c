#include "codexion.h"

static void print_error(char *str)
{
    fprintf(stderr, "Error: %s\n", str);
    exit (1);
}

static int  is_unsigned(char *str)
{
    int i;

    i = 0;
    if (!str || !*str)
        return (0);
    while (str[i])
    {
        if (str[i] < '0' || str[i] >'9')
            return (0);
        i++;
    }
    return (1);
}

static void load_and_validate(char **argv, t_sim *sim)
{
    sim->numbers_of_coders = atoi(argv[1]);
    sim->time_to_burnout = (long long)atoi(argv[2]);
    sim->time_to_compile = (long long)atoi(argv[3]);
    sim->time_to_debug = (long long)atoi(argv[4]);
    sim->time_to_refactor = (long long)atoi(argv[5]);
    sim->compiles_required = atoi(argv[6]);
    sim->dongle_cooldown = (long long)atoi(argv[7]);
    sim->scheduler = (strcmp(argv[8], "edf") == 0);

    if (sim->numbers_of_coders < 1)
        print_error("numbers_of_coders must be, at least, 1.");
    if (sim->time_to_burnout < 1)
        print_error("time_to_burnout must be, at least, 1.");
    if (sim->compiles_required < 1)
        print_error("numbers_of_compiles must be, at least, 1.");
}

int parse_args(int argc, char **argv, t_sim *sim)
{
    int i;

    if (argc != 9)
        print_error("Must be exactly 8 arguments.");
    i = 1;
    while (i < 8)
    {
        if (!is_unsigned(argv[i]))
            print_error("Invalid argument.");
        i++;
    }
    if (strcmp(argv[8], "fifo") != 0 && strcmp(argv[8], "edf") != 0)
        print_error("Scheduler must be \"fifo\" or \"edf\"");
    
    load_and_validate(argv, sim);
    return (0);
}