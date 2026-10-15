#define _POSIX_C_SOURCE 200809L

#include <errno.h>
#include <sched.h>
#include <stdio.h>
#include <sys/resource.h>

static const char *nombre_politica(int politica)
{
    switch (politica) {
    case SCHED_OTHER:
        return "SCHED_OTHER";
    case SCHED_FIFO:
        return "SCHED_FIFO";
    case SCHED_RR:
        return "SCHED_RR";
    default:
        return "desconocida";
    }
}

int main(void)
{
    int politica = sched_getscheduler(0);
    if (politica == -1) {
        perror("sched_getscheduler");
        return 1;
    }

    struct sched_param parametro;
    if (sched_getparam(0, &parametro) == -1) {
        perror("sched_getparam");
        return 1;
    }

    int prioridad_minima = sched_get_priority_min(politica);
    if (prioridad_minima == -1) {
        perror("sched_get_priority_min");
        return 1;
    }

    int prioridad_maxima = sched_get_priority_max(politica);
    if (prioridad_maxima == -1) {
        perror("sched_get_priority_max");
        return 1;
    }

    errno = 0;
    int valor_nice = getpriority(PRIO_PROCESS, 0);
    if (valor_nice == -1 && errno != 0) {
        perror("getpriority");
        return 1;
    }

    printf("Política de planificación: %s\n", nombre_politica(politica));
    printf("Prioridad actual (sched_getparam): %d\n", parametro.sched_priority);
    printf("Prioridad mínima para la política: %d\n", prioridad_minima);
    printf("Prioridad máxima para la política: %d\n", prioridad_maxima);
    printf("Valor nice actual (getpriority): %d\n", valor_nice);

    return 0;
}