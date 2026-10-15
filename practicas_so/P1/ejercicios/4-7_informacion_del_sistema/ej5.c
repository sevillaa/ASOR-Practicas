#include <sys/utsname.h>
#include <stdio.h>

int main() {
    struct utsname info;

    if(uname(&info) == -1) {
        perror("uname");
        return 1;
    }
    
    printf("Sistema operativo: %s\n", info.sysname);
    printf("Nombre del nodo: %s\n", info.nodename);
    printf("Versión del kernel: %s\n", info.release);
    printf("Versión del sistema: %s\n", info.version);
    printf("Arquitectura: %s\n", info.machine);

    return 0;
}