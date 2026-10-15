# Práctica 1

Los programas y respuestas están en `ejercicios/`, agrupados como el enunciado.
Los ejercicios 4, 8 y 11 son pruebas de comandos; no necesitan un programa C.

Desde la carpeta de un ejercicio, recompila antes de probarlo:

```bash
gcc -Wall -Wextra ej13.c -o ej13
./ej13
```

En el 1 y el 2, ejecutar como usuario normal permite observar el error de
`setuid(0)`. Si se ejecutan como root, la llamada puede funcionar sin mostrar errores.
El 13 incrementa una variable un millón de veces y calcula la diferencia completa
en microsegundos. `volatile` evita que el compilador elimine el trabajo que medimos.

Para probar el idioma de las fechas (ejercicio 15), consulta `locale -a`.
Si quieres usar la configuración del entorno en el programa, añade `#include <locale.h>`
y llama a `setlocale(LC_ALL, "")` antes de `strftime()`.
