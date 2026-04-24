#include "codexion.h"

void    enqueue(t_dongle *dongle, t_coder *coder)
{
    dongle->queue[dongle->queue_size] = coder;
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