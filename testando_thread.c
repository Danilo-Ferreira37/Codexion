#include "codexion.h"

int main(int argc, char** argv)
{
    char *teste;

    teste = malloc(0);
    if (teste)
        printf("deu certo: %s", teste);
    else
        printf("deu errado");
}

void compile(t_coder *coder, t_info_simulation infos)
{
    int left_d_is_locked;
    int right_d_is_locked;

    left_d_is_locked = pthread_mutex_trylock(&coder->left_dongle->lock);
    right_d_is_locked = pthread_mutex_trylock(&coder->right_dongle->lock);
    if (!left_d_is_locked)
    {
        printf("%d %d has taken a dongle\n", current_milliseconds(infos), coder->code_id);
        coder->has_l_dongle = 1;
    }
    if (!right_d_is_locked)
    {
        printf("%d %d has taken a dongle\n", current_milliseconds(infos), coder->code_id);
        coder->has_r_dongle = 1;
    }
    if ((left_d_is_locked || !coder->has_l_dongle) || (right_d_is_locked || !coder->has_r_dongle))
        return;

    printf("%d %d is compiling\n", current_milliseconds(infos), coder->code_id);
    time_wait(infos.time_to_copile);

    coder->total_copiles++;
    coder->time_without_copile = 0;
    
    pthread_cond_timedwait(&coder->left_dongle->lock, infos.dongle_cooldown);
    pthread_cond_timedwait(&coder->right_dongle->lock, infos.dongle_cooldown);
}
/*     int i = 0;
    while (i < info_simulation.number_of_coders)
    {
        printf("left dongle: %d, coder: %d, right dongle: %d\n", list_of_coders->left_dongle->dongle_id, list_of_coders->code_id, list_of_coders->right_dongle->dongle_id);
        list_of_coders = list_of_coders->right_coder;
        i++;
    } */
/*     while(self)
    {
        printf("left dongle %d\n coder %d\nright dongle %d\n", self->left_dongle->dongle_id, self->code_id, self->right_dongle->dongle_id);
        self = self->right_coder;
        printf("\n");
    }
    exit(0);
 */