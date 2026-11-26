#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <time.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>

#define BUF_SIZE 80

int main(int argc, char *argv[]) {
    if (argc != 3) {
        fprintf(stderr, "Uso: %s <dirección> <puerto>\n", argv[0]);
        return EXIT_FAILURE;
    }

    struct addrinfo hints;
    struct addrinfo *result;

    // 1. Configurar filtros para aceptar IPv4 e IPv6 sobre UDP
    memset(&hints, 0, sizeof(struct addrinfo));
    hints.ai_flags    = AI_PASSIVE;
    hints.ai_family   = AF_UNSPEC;   // IPv4 o IPv6
    hints.ai_socktype = SOCK_DGRAM;  // UDP

    int rc = getaddrinfo(argv[1], argv[2], &hints, &result);
    if (rc != 0) {
        fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(rc));
        return EXIT_FAILURE;
    }

    // 2. Crear el socket y asociarlo a la dirección y puerto
    int sd = socket(result->ai_family, result->ai_socktype, result->ai_protocol);
    if (sd == -1) {
        perror("socket");
        freeaddrinfo(result);
        return EXIT_FAILURE;
    }

    if (bind(sd, result->ai_addr, result->ai_addrlen) == -1) {
        perror("bind");
        close(sd);
        freeaddrinfo(result);
        return EXIT_FAILURE;
    }

    freeaddrinfo(result);

    // 3. Bucle principal de recepción y respuesta
    char buf[BUF_SIZE];
    char host[NI_MAXHOST];
    char serv[NI_MAXSERV];
    struct sockaddr_storage client_addr;
    socklen_t client_len;

    int running = 1;
    while (running) {
        // Obligatorio reinicializar client_len en cada iteración (es valor-resultado)
        client_len = sizeof(client_addr);

        ssize_t bytes = recvfrom(sd, buf, BUF_SIZE - 1, 0,
                                 (struct sockaddr *) &client_addr, &client_len);
        if (bytes == -1) {
            perror("recvfrom");
            continue;
        }
        buf[bytes] = '\0';

        // Obtener IP y puerto del cliente en formato texto
        rc = getnameinfo((struct sockaddr *) &client_addr, client_len,
                         host, NI_MAXHOST, serv, NI_MAXSERV,
                         NI_NUMERICHOST | NI_NUMERICSERV);
        if (rc != 0) {
            fprintf(stderr, "getnameinfo: %s\n", gai_strerror(rc));
            continue;
        }

        printf("%zd bytes de %s:%s\n", bytes, host, serv);

        // Obtener hora/fecha actual del sistema
        time_t now = time(NULL);
        struct tm *tm_info = localtime(&now);
        char resp[BUF_SIZE];
        size_t len = 0;

        switch (buf[0]) {
            case 't':
                len = strftime(resp, sizeof(resp), "%I:%M:%S %p", tm_info);
                if (sendto(sd, resp, len, 0,
                           (struct sockaddr *) &client_addr, client_len) == -1) {
                    perror("sendto");
                }
                break;

            case 'd':
                len = strftime(resp, sizeof(resp), "%Y-%m-%d", tm_info);
                if (sendto(sd, resp, len, 0,
                           (struct sockaddr *) &client_addr, client_len) == -1) {
                    perror("sendto");
                }
                break;

            case 'q':
                printf("Saliendo...\n");
                running = 0;
                break;

            default:
                printf("Comando %c no soportado\n", buf[0]);
                break;
        }
    }

    close(sd);
    return EXIT_SUCCESS;
}