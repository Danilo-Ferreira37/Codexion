#include "codexion.h"

void debug(t_coder *self, t_info_simulation *info)
{
    pthread_mutex_lock(&info->printl->lock);
    info->printl->print("%d %d is debugging\n", current_milliseconds(info), self->code_id);
    pthread_mutex_unlock(&info->printl->lock);
    time_wait(info->time_to_debug);
}

void refactor(t_coder *self, t_info_simulation *info)
{
    pthread_mutex_lock(&info->printl->lock);
    info->printl->print("%d %d is refactoring\n", current_milliseconds(info), self->code_id);
    pthread_mutex_unlock(&info->printl->lock);
    time_wait(info->time_to_refactor);
}

void append_queue(int *queue, int coder_id)
{   
    if (queue[0] == coder_id || queue[1] == coder_id)
        return;

    if (queue[0])
        queue[1] = coder_id;
    else
        queue[0] = coder_id;
}

int popleft_queue(int *queue)
{
    int out;

    if (!queue)
        return (-1);
    out = queue[0];
    if (queue[1])
    {
        queue[0] = queue[1];
        queue[1] = 0;
    }
    else
        queue[0] = 0;
    return (out);

}
