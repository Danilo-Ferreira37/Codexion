#include "codexion.h"

void    join_threads(t_info_simulation *info)
{
    t_coder *tmp;

    pthread_join(info->supervisor->thread, NULL);
    tmp = info->list_of_coders;
    while (1)
    {
        pthread_join(tmp->thread, NULL);
        if (tmp->code_id == info->number_of_coders)
            break;
        tmp = tmp->right_coder;
    }
}

void create_threads(t_info_simulation *infos)
{
    t_coder *thread;

    pthread_create(&infos->supervisor->thread, NULL, supervision, infos);
	thread = infos->list_of_coders;
    while (1)
    {
        t_thread_args *args = malloc(sizeof(t_thread_args));
        args->coder = thread;
        args->infos = infos;

        pthread_create(&thread->thread, NULL, thread_algoritm, args);
        if (thread->code_id == infos->number_of_coders)
            break;
        thread = thread->right_coder;
	}
}

void *supervision(void *information)
{
    t_info_simulation *info;
    t_coder *coder;

    info = (t_info_simulation *) information;
    coder = info->list_of_coders;
    while (info->running)
    {
        //pthread_mutex_lock(&info)
        if (coder->died || info->qnty_coders_comp == info->number_of_coders)
            info->running = 0;
        coder = coder->right_coder;
    }
    return (NULL);
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
    while (info->running)
    {
        if (self->total_compiles == info->number_of_compiles_required)
            break;
        if (try_compile(self, info))
        {
            debug(self, info);
            refactor(self, info);
        }
    }
    pthread_mutex_lock(&info->lock);
    info->qnty_coders_comp++;
    pthread_mutex_unlock(&info->lock);
    return (NULL);
}
