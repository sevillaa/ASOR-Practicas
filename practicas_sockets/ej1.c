#include <sys/socket.h>
#include <netdb.h>
#include <stdio.h>
/*
Escribir un programa que obtenga todas las posibles direcciones con las que se podría 
crear un socket asociado a un host dado como primer argumento del programa. Para cada dirección
mostrar la IP numérica, la familia de protocolos y tipo de socket. Comprobar el resultado para:

    - Una dirección IPv4 válida (ej. “147.96.1.9”).
    - Una dirección IPv6 válida (ej. “fd00::a:0:0:0:1”).
    - Un nombre de dominio válido (ej. “www.google.com”).
    - Un nombre en /etc/hosts válido (ej. “localhost”).
    - Una dirección o nombre incorrectos en cualquiera de los casos anteriores.

    El programa se implementará usando getaddrinfo(3) para obtener la lista de posibles direcciones
    de socket (struct sockaddr). Cada dirección se imprimirá en su valor numérico, usando getnameinfo(3)
    con el flag NI_NUMERICHOST, así como la familia de direcciones y el tipo de socket.
*/

int main(int argc, char *argv[])
{
    if (argc != 2) {
        // El programa aceptara exactamente un host.
        fprintf(stderr, "Uso: %s <host>\n", argv[0]);
        return 1;
    }
    //¿Qué direcciones queremos buscar?
    struct addrinfo criterios = {0};
    criterios.ai_family = AF_UNSPEC;

    struct addrinfo *lista = NULL;
    int error = getaddrinfo(argv[1], NULL, &criterios, &lista);
    //argv[1] -> host que nos pasan por argumento
    //NULL -> porque no nos interesa el puerto, solo la dirección
    //Criterios es lo que acepta por eso le asignamos AF_UNSPEC para que acepta ipv4 e ipv6  
    //lista -> donde se guardan los resultados de la búsqueda
    if (error != 0) {
        fprintf(stderr, "Error de getaddrinfo: %s\n", gai_strerror(error));
        return 1;
    }

    printf("Se han encontrado direcciones para %s\n", argv[1]);

    /* Siguiente paso: recorrer la lista e imprimir cada resultado. */
    struct addrinfo *actual = lista;

    while (actual != NULL) {
        char ip[NI_MAXHOST];

        error = getnameinfo(actual->ai_addr, actual->ai_addrlen,ip, sizeof(ip),NULL, 0, NI_NUMERICHOST);
        // actual->ai_addr -> dirección de socket que queremos convertir a IP numérica
        // actual->ai_addrlen -> longitud de la dirección de socket
        // ip -> buffer donde se guardará la IP numérica
        // sizeof(ip) -> tamaño del buffer ip
        // NULL -> no nos interesa el nombre del servicio
        // 0 -> tamaño del buffer del nombre del servicio
        // NI_NUMERICHOST -> IP en formato numérico, por ejemplo "127.0.0.1" en vez de "localhost".

        //* actual->ai_addrlen mide la dirección que entra y sizeof(ip) mide el espacio donde se escribirá el texto que sale.

        if (error != 0) {
            fprintf(stderr, "Error de getnameinfo: %s\n", gai_strerror(error));
            freeaddrinfo(lista);
            return 1;
        }

        printf("IP: %s, familia: %d, tipo de socket: %d\n",
            ip, actual->ai_family, actual->ai_socktype);

        actual = actual->ai_next;
    }


    freeaddrinfo(lista);
    return 0;
}
