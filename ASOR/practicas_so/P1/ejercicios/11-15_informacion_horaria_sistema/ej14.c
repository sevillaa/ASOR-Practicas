#include <time.h>
#include <stdio.h>
#include <stdlib.h>

int main() {
    
    // Necesario para llamada a localtime
    time_t t;
    time(&t);
    
    struct tm *tiempo;
    tiempo = localtime(&t);
    
    printf("AÑO: %d\n",(tiempo->tm_year)+1900);
}