#define _XOPEN_SOURCE 700
#include <fcntl.h>
#include <stdio.h>
#include <time.h>
#include <unistd.h>

int main(int argc, char **argv)
{
    if (argc != 2) {
        fprintf(stderr, "Uso: %s fichero\n", argv[0]);
        return 1;
    }
    int fd = open(argv[1], O_RDWR | O_CREAT, 0644);
    if (fd == -1 || lockf(fd, F_TLOCK, 0) == -1) {
        perror("open/lockf");
        return 1;
    }
    time_t ahora = time(NULL);
    printf("Bloqueado: %s", ctime(&ahora));
    fflush(stdout);
    sleep(10);
    if (lockf(fd, F_ULOCK, 0) == -1) {
        perror("lockf");
        return 1;
    }
    puts("Desbloqueado; espero otros 10 segundos.");
    fflush(stdout);
    sleep(10);
    close(fd);
    return 0;
}
