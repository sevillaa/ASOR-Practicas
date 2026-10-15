#define _POSIX_C_SOURCE 200809L
#include <dirent.h>
#include <limits.h>
#include <stdio.h>
#include <sys/stat.h>
#include <unistd.h>

int main(int argc, char **argv)
{
    if (argc != 2) {
        fprintf(stderr, "Uso: %s directorio\n", argv[0]);
        return 1;
    }
    if (chdir(argv[1]) == -1) {
        perror("chdir");
        return 1;
    }
    DIR *dir = opendir(".");
    if (dir == NULL) {
        perror("opendir");
        return 1;
    }
    struct dirent *entrada;
    long long total = 0;
    while ((entrada = readdir(dir)) != NULL) {
        struct stat st;
        if (lstat(entrada->d_name, &st) == -1) {
            perror("lstat");
            closedir(dir);
            return 1;
        }
        printf("%s", entrada->d_name);
        if (S_ISREG(st.st_mode)) {
            total += st.st_size;
            if (st.st_mode & 0111) printf("*");
        } else if (S_ISDIR(st.st_mode)) {
            printf("/");
        } else if (S_ISLNK(st.st_mode)) {
            char destino[PATH_MAX];
            ssize_t n = readlink(entrada->d_name, destino, sizeof(destino) - 1);
            if (n == -1) {
                perror("readlink");
                closedir(dir);
                return 1;
            }
            destino[n] = '\0'; // readlink no añade el terminador.
            printf(" -> %s", destino);
        }
        printf("\n");
    }
    closedir(dir);
    printf("Total de ficheros regulares: %.2f KB (1 KB = 1024 bytes)\n", total / 1024.0);
    return 0;
}
