#include "codexion.h"

void    join_threads(t_info_simulation *info)
{
    t_coder *tmp;

    pthread_join(info->supervisor->thread, NULL);
    tmp = info->list_of_coders;
    while (1)
    {
        pthread_join(tmp->thread, NULL);
        if (tmp->coder_id == info->number_of_coders)
            break;
        tmp = tmp->right_coder;
    }
}

void create_threads(t_info_simulation *infos)
{
    t_coder *thread;
    t_thread_args *args;

    pthread_create(&infos->supervisor->thread, NULL, supervision, infos);
	thread = infos->list_of_coders;
    while (1)
    {
        args = malloc(sizeof(t_thread_args));
        args->coder = thread;
        args->infos = infos;

        pthread_create(&thread->thread, NULL, thread_algoritm, args);
        if (thread->coder_id == infos->number_of_coders)
            break;
        thread = thread->right_coder;;
	}
}

void wakeup_all_threads(t_dongle **dongles)
{
    int i;

    i = -1;
    while (dongles[++i])
    {
        pthread_mutex_lock(&dongles[i]->lock);
        pthread_cond_broadcast(&dongles[i]->cond);
        pthread_mutex_unlock(&dongles[i]->lock);
    }
}
void *supervision(void *information)
{
    t_info_simulation *info;
    t_coder *coder;

    info = (t_info_simulation *) information;
    coder = info->list_of_coders;
    while (1)
    {
        pthread_mutex_lock(&info->lock);
        if (info->someone_dies || info->qnty_coders_comp == info->number_of_coders)
        {
            info->running = 0;
            pthread_mutex_unlock(&info->lock);
            break;
        }
        pthread_mutex_unlock(&info->lock);
        coder = coder->right_coder;
        time_wait(200);
    }
    wakeup_all_threads(info->dongles);
    return (NULL);
}

void *thread_algoritm(void *args)
{
    t_thread_args *thread_args;
    t_info_simulation *info;
    t_coder *self;
    int cond;

    thread_args = (t_thread_args *)args;
    self = thread_args->coder;
    info = thread_args->infos;
    free(args);
    while (info->running)
    {
        if (self->total_compiles == info->number_of_compiles_required)
            break;
        cond = try_compile(self, info);
        if (cond == -1)
            break;
        else if (cond)
            if (debug_and_refactor(self, info))
                break;
    }
    pthread_mutex_lock(&info->lock);
    if (!info->someone_dies)
        info->qnty_coders_comp++;
    pthread_mutex_unlock(&info->lock);
    return (NULL);
}
