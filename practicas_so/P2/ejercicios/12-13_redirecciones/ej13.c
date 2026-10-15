#include <fcntl.h>
#include <stdio.h>
#include <unistd.h>

int main(int argc, char **argv)
{
    if (argc != 2) {
        fprintf(stderr, "Uso: %s fichero\n", argv[0]);
        return 1;
    }
    int fd = open(argv[1], O_WRONLY | O_CREAT | O_TRUNC, 0644);
    if (fd == -1 || dup2(fd, STDOUT_FILENO) == -1 ||
        dup2(fd, STDERR_FILENO) == -1) {
        perror("open/dup2");
        return 1;
    }
    close(fd);
    printf("Salida normal 1\n");
    fflush(stdout); // Vaciar el búfer para conservar el orden de los mensajes.
    fprintf(stderr, "Salida de error 1\n");
    printf("Salida normal 2\n");
    fflush(stdout);
    fprintf(stderr, "Salida de error 2\n");
    return 0;
}
