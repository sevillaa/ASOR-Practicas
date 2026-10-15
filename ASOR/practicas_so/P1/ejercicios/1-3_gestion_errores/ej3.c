#include <stdio.h>
#include <string.h>

int main(void) {
    for (int codigo = 0; codigo <= 255; codigo++) {
        printf("%d: %s\n", codigo, strerror(codigo));
    }

    return 0;
}