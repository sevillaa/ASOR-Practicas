#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>

int main(int argc, char **argv)
{
    if (argc != 2) {
        fprintf(stderr, "Uso: %s \"comando argumentos\"\n", argv[0]);
        return 1;
    }
    int estado = system(argv[1]);
    if (estado == -1) {
        perror("system");
        return 1;
    }
    puts("El comando terminó de ejecutarse");
    return WIFEXITED(estado) ? WEXITSTATUS(estado) : 1;
}
