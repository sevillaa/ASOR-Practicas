#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <time.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/wait.h>
#include <netdb.h>

#define BUF_SIZE  80
#define NUM_PROCS 3

void atender_peticiones(int sd) {
    char buf[BUF_SIZE];
    char resp[BUF_SIZE];
    char host[NI_MAXHOST];
    char serv[NI_MAXSERV];
    struct sockaddr_storage client_addr;
    socklen_t client_len;

    while (1) {
        client_len = sizeof(client_addr);

        ssize_t bytes = recvfrom(sd, buf, BUF_SIZE - 1, 0,
                                 (struct sockaddr *) &client_addr, &client_len);
        if (bytes == -1) {
            perror("recvfrom");
            continue;
        }
        buf[bytes] = '\0';

        int rc = getnameinfo((struct sockaddr *) &client_addr, client_len,
                             host, NI_MAXHOST, serv, NI_MAXSERV,
                             NI_NUMERICHOST | NI_NUMERICSERV);
        if (rc != 0) {
            fprintf(stderr, "getnameinfo: %s\n", gai_strerror(rc));
            continue;
        }

        // Imprimimos el PID del hijo que ha atendido el datagrama
        printf("[PID %d] %zd bytes de %s:%s\n", getpid(), bytes, host, serv);
        fflush(stdout);

        time_t now = time(NULL);
        struct tm *tm_info = localtime(&now);
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
                printf("[PID %d] Saliendo...\n", getpid());
                fflush(stdout);
                // Envía SIGTERM a todos los procesos del grupo (padre e hijos)
                kill(0, SIGTERM);
                return;

            default:
                printf("[PID %d] Comando %c no soportado\n", getpid(), buf[0]);
                fflush(stdout);
                break;
        }
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

    printf("Servidor padre iniciado (PID/PGID: %d). Lanzando %d hijos...\n",
           getpid(), NUM_PROCS);

    // Crear el pool de procesos después de bind()
    for (int i = 0; i < NUM_PROCS; i++) {
        pid_t pid = fork();
        if (pid == -1) {
            perror("fork");
            kill(0, SIGTERM);
            return EXIT_FAILURE;
        } else if (pid == 0) {
            // Código del hijo: atiende peticiones y termina sin seguir el bucle
            atender_peticiones(sd);
            close(sd);
            exit(EXIT_SUCCESS);
        }
    }

    // Proceso padre: cierra su copia del socket y espera a los hijos
    close(sd);
    for (int i = 0; i < NUM_PROCS; i++) {
        wait(NULL);
    }

    return EXIT_SUCCESS;
}