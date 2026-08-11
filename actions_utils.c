#include "codexion.h"

static int debug(t_coder *self, t_info_simulation *info)
{
    if (thread_dies(self, info))
        return (-1);
    printl("%d %d is debugging\n", info, self->coder_id);
    time_wait(info->time_to_debug);
    return (0);
}

static int refactor(t_coder *self, t_info_simulation *info)
{
    if (thread_dies(self, info))
        return (-1);
    printl("%d %d is refactoring\n", info, self->coder_id);
    time_wait(info->time_to_refactor);
    return (0);
}

int debug_and_refactor(t_coder *self, t_info_simulation *info)
{
    if (debug(self, info) || refactor(self, info))
        return (-1);
    return (0);
}

void printl(char *message, t_info_simulation *info, int coder_id)
{
    pthread_mutex_lock(&info->lock);
    if (info->someone_dies || !info->running)
    {
        pthread_mutex_unlock(&info->lock);
        return;
    }
    pthread_mutex_lock(&info->printl->lock);
    info->printl->print(message, current_milliseconds(info), coder_id);
    pthread_mutex_unlock(&info->printl->lock);
    pthread_mutex_unlock(&info->lock);
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
