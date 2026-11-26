#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>

#define BUF_SIZE 80

int main(int argc, char *argv[]) {
    if (argc != 4) {
        fprintf(stderr, "Uso: %s <dirección> <puerto> <comando>\n", argv[0]);
        return EXIT_FAILURE;
    }

    struct addrinfo hints;
    struct addrinfo *result;

    memset(&hints, 0, sizeof(struct addrinfo));
    hints.ai_flags    = 0;
    hints.ai_family   = AF_UNSPEC;   // IPv4 o IPv6
    hints.ai_socktype = SOCK_DGRAM;  // UDP

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

    // Enviar el comando al servidor
    ssize_t sent = sendto(sd, argv[3], strlen(argv[3]), 0,
                          result->ai_addr, result->ai_addrlen);
    if (sent == -1) {
        perror("sendto");
        freeaddrinfo(result);
        close(sd);
        return EXIT_FAILURE;
    }

    // Liberar result solo DESPUÉS de usar result->ai_addr en sendto()
    freeaddrinfo(result);

    // Esperar respuesta solo si el comando genera contestación del servidor
    if (argv[3][0] == 't' || argv[3][0] == 'd') {
        char buf[BUF_SIZE];
        ssize_t bytes = recvfrom(sd, buf, BUF_SIZE - 1, 0, NULL, NULL);
        if (bytes == -1) {
            perror("recvfrom");
            close(sd);
            return EXIT_FAILURE;
        }
        buf[bytes] = '\0';
        printf("%s\n", buf);
    }

    close(sd);
    return EXIT_SUCCESS;
}

