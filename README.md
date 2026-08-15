https://pubs.opengroup.org/onlinepubs/7908799/xsh/pthread.h.html

https://dev.to/pliniohr/threads-na-linguagem-c-598k

    while (info_simulation.list_of_coders->coder_id < info_simulation.number_of_coders)
    {
        printf("d left %d  coder %d  d right %d\n", info_simulation.list_of_coders->left_dongle->dongle_id, info_simulation.list_of_coders->coder_id, info_simulation.list_of_coders->right_dongle->dongle_id);
        info_simulation.list_of_coders = info_simulation.list_of_coders->right_coder;
    }
    printf("d left %d  coder %d  d right %d\n", info_simulation.list_of_coders->left_dongle->dongle_id, info_simulation.list_of_coders->coder_id, info_simulation.list_of_coders->right_dongle->dongle_id);
    exit(0);
    //printf("\nnumero de coders que copilaram %d || compiles requeridos %d\n", info->qnty_coders_comp, info->number_of_compiles_required);
//printf("\nultima vez que o dongle foi solto: %d\ncooldown dongle %d\n tempo atual: %d\n", self->left_dongle->released_ms, info->dongle_cooldown, current_milliseconds(info));
➜  Codexion git:(main) ✗ ccf -pthread *.c

➜  Codexion git:(main) ✗ valgrind --tool=helgrind --track-lockorders=yes ./a.out 8 8000 1000 500 500 1 3
    //printf("\ntempo do comeco %d   tempo atual %d\n", infos->start_ms, current_ms);
        if (info->number_of_coders == self->coder_id)
    {
        if (try_get_dongle(self, self->left_dongle, info))
            return (-1);
        if (try_get_dongle(self, self->right_dongle, info))
            return (-1);
    }
    else
    {
        if (try_get_dongle(self, self->right_dongle, info))
            return (-1);
        if (try_get_dongle(self, self->left_dongle, info))
            return (-1);
    }