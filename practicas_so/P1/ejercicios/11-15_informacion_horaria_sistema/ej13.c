#include <stdio.h>
#include <sys/time.h>

int main() {
    struct timeval tiempo1;
    struct timeval tiempo2;
    if (gettimeofday(&tiempo1, NULL) == -1) {
        perror("gettimeofday");
        return 1;
    }
    volatile int suma = 0; // Evita que el compilador elimine el bucle.
    for (int i = 0; i < 1000000;i++){
        suma++;
    }
    if (gettimeofday(&tiempo2, NULL) == -1) {
        perror("gettimeofday");
        return 1;
    }

    long microsegundos = (tiempo2.tv_sec - tiempo1.tv_sec) * 1000000L
                        + tiempo2.tv_usec - tiempo1.tv_usec;
    printf("Tiempo: %ld microsegundos\n", microsegundos);
    return 0;
}
