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
/*     int i = 0;
    while (i < info_simulation.number_of_coders)
    {
        printf("left dongle: %d, coder: %d, right dongle: %d\n", list_of_coders->left_dongle->dongle_id, list_of_coders->code_id, list_of_coders->right_dongle->dongle_id);
        list_of_coders = list_of_coders->right_coder;
        i++;
    } */
//estudar mais sobre phtread_join()