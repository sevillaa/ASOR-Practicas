//Tue Nov  7 10:34:51 CET 2023

#include <time.h>
#include <stdio.h>
#include <stdlib.h>

int main() {
    
    // Necesario para llamada a localtime
    time_t t;
    time(&t);
    
    struct tm *tiempo;
    tiempo = localtime(&t);

    char texto[40];//Con 30 cabe justo, ponemos 40 por si pudiese variar alguna longitud de caracteres
    strftime(texto, 40, "%a %b %d %H:%M:%S %Z %Y", tiempo);

    printf("%s\n",texto);
}