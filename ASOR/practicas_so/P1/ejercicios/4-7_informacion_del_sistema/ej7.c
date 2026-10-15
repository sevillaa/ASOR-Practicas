#include <stdio.h>
#include <unistd.h>

int main() {
    long config;
    config = pathconf(".",_PC_LINK_MAX);

    if(config != -1){
        printf("El número máximo de enlaces: %ld\n", config);
    }else{
        perror("_PC_LINK_MAX");
    }

    config = pathconf(".",_PC_PATH_MAX);
    if(config != -1){
        printf("El tamaño máximo de una ruta : %ld\n", config);
    }else{
        perror("_PC_PATH_MAX");
    }

    config = pathconf(".",_PC_NAME_MAX);
    if(config != -1){
        printf("El tamaño máximo de un nombre de fichero: %ld\n",config);
    }else{
        perror("_PC_NAME_MAX");
    }


    //el número máximo de enlaces -> _PC_LINK_MAX
    //el tamaño máximo de una ruta -> _PC_PATH_MAX
    //tamaño máximo de un nombre de fichero -> _PC_NAME_MAX
    return 0;
}