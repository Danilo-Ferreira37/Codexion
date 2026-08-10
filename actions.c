#include "codexion.h"

static void get_dongle(int coder_id, t_dongle *dongle, t_info_simulation *info)
{
        dongle->owner = coder_id;
        pthread_mutex_lock(&info->printl->lock);
        info->printl->print("%d %d has taken a dongle\n", current_milliseconds(info), coder_id);
        pthread_mutex_unlock(&info->printl->lock);
}

static int compile(t_coder *self, t_info_simulation *info)
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

void try_get_dongle(int coder_id, t_dongle *dongle, t_info_simulation *info)
{
    if (dongle->owner == coder_id)
        return;
    else if (!dongle->owner && !dongle->waiting_queue[0] && (current_milliseconds(info) > dongle->released_ms + info->dongle_cooldown || !dongle->released_ms))
        get_dongle(coder_id, dongle, info);
    else
    {
        if (dongle->waiting_queue[0] == coder_id && !dongle->owner && (current_milliseconds(info) > dongle->released_ms + info->dongle_cooldown || !dongle->released_ms))
            get_dongle(popleft_queue(dongle->waiting_queue), dongle, info);
        else
        {
            if (dongle->waiting_queue[0] != coder_id && dongle->waiting_queue[1] != coder_id)
                append_queue(dongle->waiting_queue, coder_id);
            if (dongle->owner)
                pthread_cond_wait(&dongle->cond, &dongle->lock);
        }
    }
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
