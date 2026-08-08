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
        queue[1] = 0;
    }
    else
        queue[0] = 0;
    return (out);

}

int compile(t_coder *self, t_info_simulation *info)
{
    pthread_mutex_lock(&self->left_dongle->lock);
    //printf("\nultima vez que o dongle foi solto: %d\ncooldown dongle %d\n tempo atual: %d\n", self->left_dongle->released_ms, info->dongle_cooldown, current_milliseconds(info));
    if (!self->left_dongle->owner && !self->left_dongle->waiting_queue[0] && (current_milliseconds(info) > self->left_dongle->released_ms + info->dongle_cooldown || !self->left_dongle->released_ms))
    {
        self->left_dongle->owner = self->code_id;
        pthread_mutex_lock(&info->printl->lock);
        info->printl->print("%d %d has taken a dongle\n", current_milliseconds(info), self->code_id);
        pthread_mutex_unlock(&info->printl->lock);
    }
    else if (self->left_dongle->owner == self->code_id);
    else
    {
        if (self->left_dongle->waiting_queue[0] == self->code_id && !self->left_dongle->owner && (current_milliseconds(info) > self->left_dongle->released_ms + info->dongle_cooldown || !self->left_dongle->released_ms))
        {
            self->left_dongle->owner = popleft_queue(self->left_dongle->waiting_queue);
            pthread_mutex_lock(&info->printl->lock);
            info->printl->print("%d %d has taken a dongle\n", current_milliseconds(info), self->code_id);
            pthread_mutex_unlock(&info->printl->lock);
        }
        else
        {
            append_queue(self->left_dongle->waiting_queue, self->code_id);
            pthread_cond_wait(&self->left_dongle->cond, &self->left_dongle->lock);
        }
    }
    pthread_mutex_unlock(&self->left_dongle->lock);
//______________________________________________________________________________________________________________________________________________________________________________________________________________________________________________________________________
//______________________________________________________________________________________________________________________________________________________________________________________________________________________________________________________________________
    pthread_mutex_lock(&self->right_dongle->lock);
    if (!self->right_dongle->owner && !self->right_dongle->waiting_queue[0] && (current_milliseconds(info) > self->right_dongle->released_ms + info->dongle_cooldown || !self->right_dongle->released_ms))
    {
        self->right_dongle->owner = self->code_id;
        pthread_mutex_lock(&info->printl->lock);
        info->printl->print("%d %d has taken a dongle\n", current_milliseconds(info), self->code_id);
        pthread_mutex_unlock(&info->printl->lock);
    }
    else if (self->right_dongle->owner == self->code_id);
    else
    {
        if (self->right_dongle->waiting_queue[0] == self->code_id && !self->right_dongle->owner && (current_milliseconds(info) > self->right_dongle->released_ms + info->dongle_cooldown || !self->right_dongle->released_ms))
        {
            self->right_dongle->owner = popleft_queue(self->right_dongle->waiting_queue);
            pthread_mutex_lock(&info->printl->lock);
            info->printl->print("%d %d has taken a dongle\n", current_milliseconds(info), self->code_id);
            pthread_mutex_unlock(&info->printl->lock);
        }
        else
        {
            append_queue(self->right_dongle->waiting_queue, self->code_id);
            pthread_cond_wait(&self->right_dongle->cond, &self->right_dongle->lock);
        }
    }
    pthread_mutex_unlock(&self->right_dongle->lock);

    if (self->code_id == self->left_dongle->owner && self->code_id == self->right_dongle->owner)
    {
        pthread_mutex_lock(&info->printl->lock);
        info->printl->print("%d %d is compiling\n",  current_milliseconds(info), self->code_id);
        pthread_mutex_unlock(&info->printl->lock);
        time_wait(info->time_to_compile);

        pthread_mutex_lock(&self->right_dongle->lock);
        self->right_dongle->owner = 0;
        self->right_dongle->released_ms = current_milliseconds(info);
        pthread_cond_broadcast(&self->right_dongle->cond);
        pthread_mutex_unlock(&self->right_dongle->lock);

        pthread_mutex_lock(&self->left_dongle->lock);
        self->left_dongle->owner = 0;
        self->left_dongle->released_ms = current_milliseconds(info);
        pthread_cond_broadcast(&self->left_dongle->cond);
        pthread_mutex_unlock(&self->left_dongle->lock);
        return (1);
    }
    return (0);
}

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

void *thread_algoritm(void *args)
{
    t_thread_args *thread_args;
    t_info_simulation *info;
    t_coder *self;
    int comp;

    thread_args = (t_thread_args *)args;
    self = thread_args->coder;
    info = thread_args->infos;
    comp = 0;
    int i = 0;
    free(args);
    //if (self->code_id % 2 == 0)
    //    time_wait(500);

    while (info->running)
    {
        comp = compile(self, info);
        if (comp)
        {
            debug(self, info);
            refactor(self, info);
        }
        
    }
        return (NULL);
}
