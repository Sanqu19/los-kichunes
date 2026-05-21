#include <stdio.h>
#include <omp.h>

int main() {
    int i;
    int N = 13;

    // En este caso, OpenMP asigna bloques grandes al inicio y los
    // reduce progresivamente hasta 1. Sin chunk minimo especificado.
    printf("--- Ejemplo 1: schedule(guided) por defecto ---\n");
    #pragma omp parallel for schedule(guided) num_threads(4)
    for (i = 0; i < N; i++) {
        int hilo = omp_get_thread_num();
        printf("Hilo %d ejecuta la iteracion %d\n", hilo, i);
    }

    // En este caso, los bloques se reducen progresivamente pero
    // nunca bajan de 2 iteraciones por asignacion (chunk minimo = 2).
    printf("\n--- Ejemplo 2: schedule(guided, 2) ---\n");
    #pragma omp parallel for schedule(guided, 2) num_threads(4)
    for (i = 0; i < N; i++) {
        int hilo = omp_get_thread_num();
        printf("Hilo %d ejecuta la iteracion %d\n", hilo, i);
    }

    return 0;
}