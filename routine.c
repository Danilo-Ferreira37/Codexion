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

void try_get_dongle(int coder_id, t_dongle *dongle, t_info_simulation *info)
{
    if (!dongle->owner && !dongle->waiting_queue[0] && (current_milliseconds(info) > dongle->released_ms + info->dongle_cooldown || !dongle->released_ms))
    {
        dongle->owner = coder_id;
        pthread_mutex_lock(&info->printl->lock);
        info->printl->print("%d %d has taken a dongle\n", current_milliseconds(info), coder_id);
        pthread_mutex_unlock(&info->printl->lock);
    }
    else if (dongle->owner == coder_id);
    else
    {
        if (dongle->waiting_queue[0] == coder_id && !dongle->owner && (current_milliseconds(info) > dongle->released_ms + info->dongle_cooldown || !dongle->released_ms))
        {
            dongle->owner = popleft_queue(dongle->waiting_queue);
            pthread_mutex_lock(&info->printl->lock);
            info->printl->print("%d %d has taken a dongle\n", current_milliseconds(info), coder_id);
            pthread_mutex_unlock(&info->printl->lock);
        }
        else
        {
            if (dongle->waiting_queue[0] != coder_id && dongle->waiting_queue[1] != coder_id)
                append_queue(dongle->waiting_queue, coder_id);
            if (dongle->owner)
                pthread_cond_wait(&dongle->cond, &dongle->lock);
        }
    }
}

int compile(t_coder *self, t_info_simulation *info)
{
        pthread_mutex_lock(&info->printl->lock);
        info->printl->print("%d %d is compiling\n",  current_milliseconds(info), self->code_id);
        pthread_mutex_unlock(&info->printl->lock);
        time_wait(info->time_to_compile);
        self->total_compiles++;

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

int try_compile(t_coder *self, t_info_simulation *info)
{
    //printf("\nultima vez que o dongle foi solto: %d\ncooldown dongle %d\n tempo atual: %d\n", self->left_dongle->released_ms, info->dongle_cooldown, current_milliseconds(info));
    pthread_mutex_lock(&self->left_dongle->lock);
    try_get_dongle(self->code_id, self->left_dongle, info);
    pthread_mutex_unlock(&self->left_dongle->lock);

    pthread_mutex_lock(&self->right_dongle->lock);
    try_get_dongle(self->code_id, self->right_dongle, info);
    pthread_mutex_unlock(&self->right_dongle->lock);

    if (self->code_id == self->left_dongle->owner && self->code_id == self->right_dongle->owner)
        return (compile(self, info));
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
    while (info->running)
    {
        if (self->total_compiles == info->number_of_compiles_required)
            break;
        comp = try_compile(self, info);
        if (comp)
        {
            debug(self, info);
            refactor(self, info);
        }
    }
        return (NULL);
}
