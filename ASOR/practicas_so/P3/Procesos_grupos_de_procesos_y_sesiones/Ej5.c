#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <limits.h>

int main(int argc, char **argv){

    pid_t pid = getpid();
    pid_t ppid = getppid();
    pid_t pgid = getpgid(0);
    pid_t sid = getsid(0);

    if (pgid == -1 || sid == -1) {
        perror("Error al obtener PGID o SID");
        return 1;
    }

    long max_ficheros = sysconf(_SC_OPEN_MAX);
    if (max_ficheros == -1) {
        fprintf(stderr, "No se pudo obtener el límite de ficheros\n");
        return 1;
    }

    char directorio[PATH_MAX];
    if (getcwd(directorio, sizeof(directorio)) == NULL) {
        perror("getcwd");
        return 1;
    }

    printf("PID: %ld\nPPID: %ld\nPGID: %ld\nSID: %ld\n",
           (long)pid, (long)ppid, (long)pgid, (long)sid);
    printf("Máximo de ficheros abiertos: %ld\n", max_ficheros);
    printf("Directorio de trabajo: %s\n", directorio);

    return 0;
}