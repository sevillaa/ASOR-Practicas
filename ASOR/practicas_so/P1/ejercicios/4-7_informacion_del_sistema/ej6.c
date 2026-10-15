#include <stdio.h>
#include <unistd.h>

int main() {
    long config;
    config = sysconf(_SC_ARG_MAX);

    if(config != -1){
        printf("Longitud maxima de los argumentos: %ld\n", config);
    }else{
        perror("_SC_ARG_MAX");
    }

    config = sysconf(_SC_CHILD_MAX);
    if(config != -1){
        printf("El número máximo de hijos es: %ld\n", config);
    }else{
        perror("_SC_CHILD_MAX");
    }

    config = sysconf(_SC_OPEN_MAX);
    if(config != -1){
        printf("El número máximo de ficheros abiertos es: %ld\n",config);
    }else{
        perror("_SC_OPEN_MAX");
    }


    //la longitud máxima de los argumentos -> _SC_ARG_MAX
    //el número máximo de hijos -> _SC_CHILD_MAX
    //el número máximo de ficheros abiertos -> _SC_OPEN_MAX
    return 0;
}