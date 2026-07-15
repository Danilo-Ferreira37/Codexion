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

//estudar mais sobre phtread_join()