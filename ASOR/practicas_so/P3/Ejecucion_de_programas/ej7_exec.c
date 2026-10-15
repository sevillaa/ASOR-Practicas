#include <stdio.h>
#include <unistd.h>

int main(int argc, char **argv)
{
    if (argc < 2) {
        fprintf(stderr, "Uso: %s comando [argumentos]\n", argv[0]);
        return 1;
    }
    execvp(argv[1], &argv[1]);

    // Solo llegamos aquí si execvp falla: si funciona, sustituye el programa.
    perror("execvp");
    puts("El comando terminó de ejecutarse");
    return 1;
}
