
#include <stdio.h>
#include <omp.h>
#include <unistd.h>   /* usleep() — simula trabajo variable */

/* Simula trabajo con coste distinto por iteración.
   Las iteraciones pares son rápidas, las impares son lentas. */
void procesar(int i) {
    if (i % 2 == 0)
        usleep(10000);   /* 10 ms — iteración rápida */
    else
        usleep(80000);   /* 80 ms — iteración lenta  */
}

int main() {
    const int N      = 20;
    const int CHUNK  = 8;
    const int HILOS  = 4;

    omp_set_num_threads(HILOS);

    printf("schedule(dynamic, %d) con %d hilos y %d iteraciones\n\n",
           CHUNK, HILOS, N);
    printf("%-6s %-8s %-10s\n", "iter", "hilo", "tipo");
    printf("%-6s %-8s %-10s\n", "------", "--------", "----------");

    double t_inicio = omp_get_wtime();

    #pragma omp parallel for schedule(dynamic, CHUNK) shared(N)
    for (int i = 0; i < N; i++) {
        int tid = omp_get_thread_num();

        #pragma omp critical
        printf("%-6d %-8d %-10s\n",
               i, tid,
               (i % 2 == 0) ? "rapida" : "lenta");

        procesar(i);
    }

    double t_total = omp_get_wtime() - t_inicio;
    printf("\nTiempo total: %.2f s\n", t_total);
    printf("Speedup aprox. vs secuencial: %.1fx\n",
           (N/2 * 0.01 + N/2 * 0.08) / t_total);

    return 0;
}