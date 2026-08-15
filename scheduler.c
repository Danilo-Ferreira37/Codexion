#include "codexion.h"

static t_coder *popleft_queue(t_coder **queue)
{
    t_coder *coder;

    if (!queue)
        return (NULL);
    coder = queue[0];
    if (queue[1])
    {
        queue[0] = queue[1];
        queue[1] = NULL;
    }
    else
        queue[0] = NULL;
    return (coder);

}

static t_coder    *fifo(t_coder **waiting_queue, int remove_chosen_coder)
{
    if (remove_chosen_coder)
        return (popleft_queue(waiting_queue));
    return (waiting_queue[0]);
}

static t_coder    *edf_remove_queue(t_coder **waiting_queue)
{
    t_coder *coder;

    if (waiting_queue[1]->last_compile < waiting_queue[0]->last_compile)
    {
        coder = waiting_queue[1];
        waiting_queue[1] = NULL;
        return (coder);
    }
    else
    {
        coder = waiting_queue[0];
        waiting_queue[0] = waiting_queue[1];
        waiting_queue[1] = NULL;
        return (coder);
    }
}

static t_coder    *edf(t_coder **waiting_queue, int remove_chosen_coder)
{
    if (remove_chosen_coder)
        return (edf_remove_queue(waiting_queue));

    if (waiting_queue[1]->last_compile < waiting_queue[0]->last_compile)
        return (waiting_queue[1]);
    else
        return (waiting_queue[0]);
}

t_coder    *scheduler(t_coder **waiting_queue, int remove_chosen_coder, t_info_simulation *infos)
{
    if (!waiting_queue[0])
        return (NULL);
    if (!waiting_queue[1] && !remove_chosen_coder)
        return (waiting_queue[0]);
    if (!waiting_queue[1] && remove_chosen_coder)
        return (popleft_queue(waiting_queue));

    if (infos->scheduler == 'F')
        return (fifo(waiting_queue, remove_chosen_coder));
    else
        return (edf(waiting_queue, remove_chosen_coder));
}
