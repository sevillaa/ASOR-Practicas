#include <time.h>
#include <stdio.h>

int main() {
    time_t tiempo;
    time(&tiempo);
    printf("TIEMPO: %ld\n",tiempo);
}