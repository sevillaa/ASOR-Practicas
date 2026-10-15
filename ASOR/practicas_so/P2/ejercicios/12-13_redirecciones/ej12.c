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
    if (fd == -1 || dup2(fd, STDOUT_FILENO) == -1) {
        perror("open/dup2");
        return 1;
    }
    close(fd);
    printf("Primera línea\n");
    printf("Segunda línea\n");
    return 0;
}
