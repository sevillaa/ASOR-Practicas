# Práctica 2

Cada ejercicio tiene su programa o respuesta en `ejercicios/`. También está el 16 opcional.
Recompila los fuentes antes de probarlos: los ejecutables antiguos pueden estar desactualizados.

Desde la carpeta del ejercicio, por ejemplo:

```bash
gcc -Wall -Wextra ej12.c -o ej12
./ej12 /tmp/asor-salida
cat /tmp/asor-salida
```

Pruebas de los programas nuevos (cada uno desde su carpeta y después de compilarlo):

| Ejercicio | Prueba | Resultado |
|---|---|---|
| 12 | `./ej12 /tmp/asor-salida` | Dos líneas en el fichero. |
| 13 | `./ej13 /tmp/asor-salidas` | Mensajes normales y de error en el mismo fichero. |
| 15 | `./ej15 /tmp/asor-cerrojo` | Bloquea 10 s, desbloquea y espera otros 10 s. |
| 17 | `./ej17 .` | Lista nombres con `*`, `/` o `-> destino`, y el total en KB. |

Las respuestas sobre el orden de las redirecciones están en `ej13.txt`.
Las pruebas de los cerrojos y las dos terminales están en `ej14.txt`, `ej15.txt` y `ej16.txt`.
El 17 solo suma los ficheros regulares del directorio indicado; no recorre subdirectorios.

En los ejercicios 5 y 7 usa un nombre de fichero nuevo: `open()` no cambia los
permisos de uno que ya existe. El 5 solicita `0645`, sujeto a la máscara heredada;
el 7 fija `0027` y crea `0640`. El 9 usa `lstat()` para reconocer enlaces.
