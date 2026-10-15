#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>

int main(int argc, char** argv){

    if (argc != 2) {
        fprintf(stderr, "Uso: %s fichero\n", argv[0]);
        return 1;
    }
    int fd = open(argv[1], O_WRONLY | O_CREAT, 0645);
    if (fd == -1) {
        perror("open");
        return 1;
    }
    close(fd);
    return 0;
}
