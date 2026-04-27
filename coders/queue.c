#include "codexion.h"

static int is_earlier_deadline(t_coder *a, t_coder *b)
{
    long long deadline_a;
    long long deadline_b;

    deadline_a = a->last_compile_start + a->sim->time_to_burnout;
    deadline_b = b->last_compile_start + b->sim->time_to_burnout;
    if (deadline_a < deadline_b)
        return (1);
    if (deadline_a > deadline_b)
        return (0);
    return (a->id < b->id);
}

void    enqueue(t_dongle *dongle, t_coder *coder)
{
    int i;
    int insert_pos;

    if (coder->sim->scheduler == 0)
    {
        dongle->queue[dongle->queue_size] = coder;
        dongle->queue_size++;
        return;
    }
    insert_pos = 0;
    while (insert_pos < dongle->queue_size
        && !is_earlier_deadline(coder, dongle->queue[insert_pos]))
        insert_pos++;
    i = dongle->queue_size;
    while (i > insert_pos)
    {
        dongle->queue[i] = dongle->queue[i - 1];
        i--;
    }
    dongle->queue[insert_pos] = coder;
    dongle->queue_size++;
}

t_coder *dequeue(t_dongle *dongle)
{
    t_coder *first;
    int     i;

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