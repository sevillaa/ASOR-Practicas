#define _POSIX_C_SOURCE 200809L
#include <stdio.h>
#include <unistd.h>
#include <limits.h>

int main(void)
{
    pid_t pid = fork();
    if (pid == -1) {
        perror("fork");
        return 1;
    }

    if (pid == 0) {
        if (setsid() == -1) {
            perror("setsid");
            return 1;
        }
        if (chdir("/tmp") == -1) {
            perror("chdir");
            return 1;
        }
        sleep(10); // Hijo: cambiar los tiempos para probar cada caso.
    } else {
        sleep(5);  // Padre: no usamos wait() para poder observar zombis.
    }

    char directorio[PATH_MAX];
    if (getcwd(directorio, sizeof(directorio)) == NULL) {
        perror("getcwd");
        return 1;
    }
    printf("%s: PID=%ld PPID=%ld PGID=%ld SID=%ld\n",
           pid == 0 ? "Hijo" : "Padre", (long)getpid(), (long)getppid(),
           (long)getpgid(0), (long)getsid(0));
    printf("Máximo de ficheros: %ld\nDirectorio: %s\n",
           sysconf(_SC_OPEN_MAX), directorio);
    return 0;
}
