https://pubs.opengroup.org/onlinepubs/7908799/xsh/pthread.h.html

https://dev.to/pliniohr/threads-na-linguagem-c-598k

    while (info_simulation.list_of_coders)
    {
        printf("d left %d  coder %d  d right %d\n", info_simulation.list_of_coders->left_dongle->dongle_id, info_simulation.list_of_coders->code_id, info_simulation.list_of_coders->right_dongle->dongle_id);
        info_simulation.list_of_coders = info_simulation.list_of_coders->right_coder;
    }
    exit(0);

➜  Codexion git:(main) ✗ ccf -pthread *.c

➜  Codexion git:(main) ✗ valgrind --tool=helgrind --track-lockorders=yes ./a.out 8 8000 1000 500 500 1 3