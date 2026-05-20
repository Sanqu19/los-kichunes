#include <stdio.h>
#include <omp.h>

int main() {
    int i;
    int N = 12; 

    printf("--- Ejemplo 1: schedule(static) por defecto ---\n");
    // En este caso, cada hilo ejecutará un bloque de iteraciones contiguas
    #pragma omp parallel for schedule(static) num_threads(4)
    for (i = 0; i < N; i++) {
        int hilo = omp_get_thread_num();
        printf("Hilo %d ejecuta la iteracion %d\n", hilo, i);
    }
    // En este caso, cada hilo ejecutará bloques de 2 iteraciones, lo que puede mejorar la carga de trabajo en algunos casos
    printf("\n--- Ejemplo 2: schedule(static, 2) ---\n");
    #pragma omp parallel for schedule(static, 2) num_threads(4)
    for (i = 0; i < N; i++) {
        int hilo = omp_get_thread_num();
        printf("Hilo %d ejecuta la iteracion %d\n", hilo, i);
    }

    return 0;
}
