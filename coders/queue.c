#include "codexion.h"

static int is_earlier_deadline(t_coder *a, t_coder *b)
{
    long long deadline1;
    long long deadline2;

    deadline1 = a->last_compile_start + a->sim->time_to_burnout; // this is the moment when the coder will die if he don't start compiling before then
    deadline2 = b->last_compile_start + b->sim->time_to_burnout;
    if (deadline1 < deadline2) // the coder with the earliest deadline should be first in the queue
        return (1);
    if (deadline1 > deadline2)
        return (0);
    return (a->id < b->id); // if they have the same deadline, the smallest one wins to avoid starvation of the coders with higher id
}

void    enqueue(t_dongle *dongle, t_coder *coder)
{
    int i;
    int position;

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
    i = dongle->queue_size;
    while (i > position) // search the position where the coder should be inserted and move all the others to the right to order the queue by deadline (EDF)
    {
        dongle->queue[i] = dongle->queue[i - 1];
        i--;
    }
    dongle->queue[position] = coder;
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
    while (i < dongle->queue_size - 1) // remove the first coder from the queue and move all the others to the left
    {
        dongle->queue[i] = dongle->queue[i + 1];
        i++;
    }
    dongle->queue_size--;
    return (first);
}
