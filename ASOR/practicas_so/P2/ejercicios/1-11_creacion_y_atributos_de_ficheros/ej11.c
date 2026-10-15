#include <stdio.h>
#include <string.h>
#include <sys/stat.h>
#include <unistd.h>

int main(int argc, char **argv)
{
    if (argc != 2) {
        fprintf(stderr, "Uso: %s fichero\n", argv[0]);
        return 1;
    }
    struct stat st;
    if (lstat(argv[1], &st) == -1) {
        perror("lstat");
        return 1;
    }
    if (!S_ISREG(st.st_mode)) {
        fprintf(stderr, "La ruta debe ser un fichero regular\n");
        return 1;
    }
    char hard[strlen(argv[1]) + 6];
    char sym[strlen(argv[1]) + 5];
    sprintf(hard, "%s.hard", argv[1]);
    sprintf(sym, "%s.sym", argv[1]);

    // El enlace se crea junto al fichero: su destino relativo es solo el nombre.
    char *nombre = strrchr(argv[1], '/');
    nombre = nombre ? nombre + 1 : argv[1];
    if (link(argv[1], hard) == -1 || symlink(nombre, sym) == -1) {
        perror("link/symlink");
        return 1;
    }
    return 0;
}
