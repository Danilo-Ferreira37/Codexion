#include "codexion.h"

void    join_threads(t_info_simulation *info)
{
    while (info->list_of_coders)
    {
        pthread_join(info->list_of_coders->thread, NULL);
        info->list_of_coders = info->list_of_coders->right_coder;
    }
}
static void append_queue(int *queue, int coder_id)
{   
    if (queue[0] == coder_id || queue[1] == coder_id)
        return;

    if (queue[0])
        queue[1] = coder_id;
    else
        queue[0] = coder_id;
}

static int popleft_queue(int *queue)
{
    int out;

    if (!queue)
        return (-1);
    out = queue[0];
    if (queue[1])
    {
        queue[0] = queue[1];
        queue[1] = 0;;
    }
    else
        queue[0] = 0;
    return (out);

}

void *thread_algoritm(void *args)
{
    t_thread_args *thread_args;
    t_info_simulation *info;
    t_coder *self;

    thread_args = (t_thread_args *)args;
    self = thread_args->coder;
    info = thread_args->infos;
    free(args);
    if (self->code_id % 2 == 0)
        time_wait(2000);
    while (info->running)
    {
        pthread_mutex_lock(&self->left_dongle->lock);
        if (!self->left_dongle->owner && !self->left_dongle->waiting_queue[0])
        {
            self->left_dongle->owner = self->code_id;
            self->has_left_dongle = 1;
        }
        else
        {
            if (self->left_dongle->waiting_queue[0] == self->code_id && !self->left_dongle->owner)
            {
                self->left_dongle->owner = popleft_queue(self->left_dongle->waiting_queue);
                self->has_left_dongle = 1;
            }
            else
            {
                append_queue(self->left_dongle->waiting_queue, self->code_id);
                pthread_cond_wait(&self->left_dongle->cond, &self->left_dongle->lock);
            }
        }
        pthread_mutex_unlock(&self->left_dongle->lock);
        //_____________________________________________________________________________________________________________________________________________________________
        pthread_mutex_lock(&self->right_dongle->lock);
        if (!self->right_dongle->owner && !self->right_dongle->waiting_queue[0])
        {
            self->right_dongle->owner = self->code_id;
            self->has_right_dongle = 1;
        }
        else
        {
            if (self->right_dongle->waiting_queue[0] == self->code_id && !self->right_dongle->owner)
            {
                self->right_dongle->owner = popleft_queue(self->right_dongle->waiting_queue);
                self->has_right_dongle = 1;
            }
            else
            {
                append_queue(self->right_dongle->waiting_queue, self->code_id);
                pthread_cond_wait(&self->right_dongle->cond, &self->right_dongle->lock);
            }
        }
        pthread_mutex_unlock(&self->right_dongle->lock);
        
        return (NULL);
}
