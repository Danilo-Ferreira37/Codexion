#include "codexion.h"

int thread_dies(t_coder *self, t_info_simulation *info)
{
    pthread_mutex_lock(&info->lock);
    if (info->someone_dies || info->qnty_coders_comp == info->number_of_compiles_required)
    {
        pthread_mutex_unlock(&info->lock);
        return(-1);
    }

    if ((current_milliseconds(info) - self->last_compile) >= info->time_to_burnout)
    {
        info->someone_dies = 1;
        printf("%d %d burned out\n", current_milliseconds(info), self->coder_id);
        pthread_mutex_unlock(&info->lock);
        return (-1);
    }
    pthread_mutex_unlock(&info->lock);
    return (0);
}

static void get_dongle(int coder_id, t_dongle *dongle, t_info_simulation *info)
{
    dongle->owner = coder_id;
    printl("%d %d has taken a dongle\n", info, coder_id);
}

static int compile(t_coder *self, t_info_simulation *info)
{
    printl("%d %d is compiling\n", info, self->coder_id);
    time_wait(info->time_to_compile);
    self->last_compile = current_milliseconds(info);
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

static int try_get_dongle(t_coder *self, t_dongle *dongle, t_info_simulation *info)
{
    if (thread_dies(self, info))
        return (-1);
    pthread_mutex_lock(&dongle->lock);
    if (dongle->owner == self->coder_id)
    {
        pthread_mutex_unlock(&dongle->lock);
        return (0);
    }
    else if (!dongle->owner && !dongle->waiting_queue[0] && (current_milliseconds(info) > dongle->released_ms + info->dongle_cooldown || !dongle->released_ms))
        get_dongle(self->coder_id, dongle, info);
    else
    {
        // vou ter que por infelizmente na waiting queue o proprio objeto coder,
        // e nao apenas o ID, porque tenho que acessar os atributos do coder como
        // por exemplo a utia vez que ele copilou para aplicar o edf
        // vai ser bem chato porque minhas funcoes append e popleft vao ter que ser completamente alteradas, : (   (emoji de choro)
        // Pelo menos so uso essas funcoes nessa mesm funcao (nao vou ter que rastrear erros) : ) (fiquei um pouquinho mais feliz)
        if (dongle->waiting_queue[0] == self->coder_id && !dongle->owner && (current_milliseconds(info) > dongle->released_ms + info->dongle_cooldown || !dongle->released_ms))
            get_dongle(popleft_queue(dongle->waiting_queue), dongle, info);
        else
        {
            if (dongle->waiting_queue[0] != self->coder_id && dongle->waiting_queue[1] != self->coder_id)
                append_queue(dongle->waiting_queue, self->coder_id);
            if (dongle->owner)
                pthread_cond_wait(&dongle->cond, &dongle->lock);
            //else
            // o tal do cond_timedwait()
        }
    }
    pthread_mutex_unlock(&dongle->lock);
    return (0);
}

int try_compile(t_coder *self, t_info_simulation *info)
{
    if (thread_dies(self, info))
        return (-1);

    // tenho uma aplicar um metodo que evita um possivel deadlock !!
    if (try_get_dongle(self, self->left_dongle, info))
        return (-1);
    if (try_get_dongle(self, self->right_dongle, info))
        return (-1);

    if (thread_dies(self, info))
        return (-1);
    if (self->coder_id == self->left_dongle->owner && self->coder_id == self->right_dongle->owner)
        return (compile(self, info));
    return (0);
}
