#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <time.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/select.h>
#include <netdb.h>

#define BUF_SIZE 80

/* Devuelve > 0 con la longitud de la respuesta, 0 si comando inválido, -1 si hay que salir */
int procesar_comando(char cmd, char *resp, size_t resp_size) {
    time_t now = time(NULL);
    struct tm *tm_info = localtime(&now);

    switch (cmd) {
        case 't':
            return (int) strftime(resp, resp_size, "%I:%M:%S %p", tm_info);
        case 'd':
            return (int) strftime(resp, resp_size, "%Y-%m-%d", tm_info);
        case 'q':
            printf("Saliendo...\n");
            return -1;
        default:
            printf("Comando %c no soportado\n", cmd);
            return 0;
    }
}

int main(int argc, char *argv[]) {
    if (argc != 3) {
        fprintf(stderr, "Uso: %s <dirección> <puerto>\n", argv[0]);
        return EXIT_FAILURE;
    }

    struct addrinfo hints;
    struct addrinfo *result;

    memset(&hints, 0, sizeof(struct addrinfo));
    hints.ai_flags    = AI_PASSIVE;
    hints.ai_family   = AF_UNSPEC;
    hints.ai_socktype = SOCK_DGRAM;

    int rc = getaddrinfo(argv[1], argv[2], &hints, &result);
    if (rc != 0) {
        fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(rc));
        return EXIT_FAILURE;
    }

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

    char buf[BUF_SIZE];
    char resp[BUF_SIZE];
    char host[NI_MAXHOST];
    char serv[NI_MAXSERV];
    struct sockaddr_storage client_addr;
    socklen_t client_len;

    int max_fd = (sd > STDIN_FILENO) ? sd : STDIN_FILENO;
    int running = 1;

    while (running) {
        // 1. Reconstruir el conjunto de lectura en CADA iteración
        fd_set readfds;
        FD_ZERO(&readfds);
        FD_SET(STDIN_FILENO, &readfds);
        FD_SET(sd, &readfds);

        // 2. Bloquear hasta que haya datos en el socket o en stdin
        int ready = select(max_fd + 1, &readfds, NULL, NULL, NULL);
        if (ready == -1) {
            perror("select");
            break;
        }

        // 3. Caso A: llegan datos por red (socket UDP)
        if (FD_ISSET(sd, &readfds)) {
            client_len = sizeof(client_addr);
            ssize_t bytes = recvfrom(sd, buf, BUF_SIZE - 1, 0,
                                     (struct sockaddr *) &client_addr, &client_len);
            if (bytes == -1) {
                perror("recvfrom");
                continue;
            }
            buf[bytes] = '\0';

            rc = getnameinfo((struct sockaddr *) &client_addr, client_len,
                             host, NI_MAXHOST, serv, NI_MAXSERV,
                             NI_NUMERICHOST | NI_NUMERICSERV);
            if (rc != 0) {
                fprintf(stderr, "getnameinfo: %s\n", gai_strerror(rc));
                continue;
            }

            printf("%zd bytes de %s:%s\n", bytes, host, serv);

            int len = procesar_comando(buf[0], resp, sizeof(resp));
            if (len > 0) {
                if (sendto(sd, resp, len, 0,
                           (struct sockaddr *) &client_addr, client_len) == -1) {
                    perror("sendto");
                }
            } else if (len == -1) {
                running = 0;
            }
        }

        // 4. Caso B: llegan datos por el terminal (entrada estándar)
        if (running && FD_ISSET(STDIN_FILENO, &readfds)) {
            ssize_t bytes = read(STDIN_FILENO, buf, 2);
            if (bytes <= 0) {
                if (bytes == -1) perror("read");
                running = 0;
            } else {
                int len = procesar_comando(buf[0], resp, sizeof(resp));
                if (len > 0) {
                    printf("%s\n", resp);
                } else if (len == -1) {
                    running = 0;
                }
            }
        }
    }

    close(sd);
    return EXIT_SUCCESS;
}