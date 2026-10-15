#define _POSIX_C_SOURCE 200809L
#include <fcntl.h>
#include <stdio.h>
#include <unistd.h>

int main(int argc, char **argv)
{
    if (argc < 2) {
        fprintf(stderr, "Uso: %s comando [argumentos]\n", argv[0]);
        return 1;
    }
    pid_t hijo = fork();
    if (hijo == -1) {
        perror("fork");
        return 1;
    }
    if (hijo > 0) {
        printf("PID del hijo: %ld\n", (long)hijo);
        return 0;
    }

    if (setsid() == -1 || chdir("/tmp") == -1) {
        perror("setsid/chdir");
        return 1;
    }
    int entrada = open("/dev/null", O_RDONLY);
    int salida = open("/tmp/daemon.out", O_WRONLY | O_CREAT | O_TRUNC, 0644);
    int error = open("/tmp/daemon.err", O_WRONLY | O_CREAT | O_TRUNC, 0644);
    if (entrada == -1 || salida == -1 || error == -1) {
        perror("open");
        return 1;
    }
    if (dup2(entrada, STDIN_FILENO) == -1 ||
        dup2(salida, STDOUT_FILENO) == -1 ||
        dup2(error, STDERR_FILENO) == -1) {
        perror("dup2");
        return 1;
    }
    close(entrada);
    close(salida);
    close(error);

    execvp(argv[1], &argv[1]);
    perror("execvp");
    return 1;
}
