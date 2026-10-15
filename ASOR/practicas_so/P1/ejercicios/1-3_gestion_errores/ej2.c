#include <unistd.h>
#include <stdio.h>
#include <errno.h>
#include <string.h>

int main() {
    
    if (setuid(0) == -1) {
        int codigo_error = errno;

        printf("Código de error: %d\n", codigo_error);
        printf("Mensaje: %s\n", strerror(codigo_error));

        return 1;
    }
    return 0;
}
