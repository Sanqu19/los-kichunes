# los-kichunes
Alumnos:
- \# 322080759- Hiram Zadkiel Gonzalez Ibañez
- \# 322182059- Angel Salvador Sandoval Quijano
- \# 322332924 - Cuevas Velazquez Luis Eduardo

---

## Objetivo
entender cómo OpenMP reparte las iteraciones de un parallel for entre los hilos, y por qué la decisión afecta el rendimiento.

---

## Descripción

---
### Schedule Static
### El Contexto: ¿Por qué necesitamos un "Schedule"?

Cuando paralelices un bucle usando la directiva `#pragma omp for`, OpenMP necesita saber qué hilo ejecutará qué iteraciones. Si tienes un bucle de 100 iteraciones y 4 hilos de procesamiento, no puedes dejar que todos hagan todo. Necesitas dividirlos. 

Aquí es donde entra el *scheduling* (planificación). Las dos estrategias principales son **estática** y **dinámica**, y la elección depende completamente de la naturaleza del trabajo dentro de tu bucle.

### La Teoría detrás de `schedule(static)`

Como su nombre lo indica, la planificación estática es **predeterminada y determinista**. La repartición del trabajo se calcula y se asigna a los hilos *antes* de que el bucle comience a ejecutarse. 

Funciona de la siguiente manera, dependiendo de si especificas o no un parámetro adicional llamado *chunk size* (tamaño de bloque):

 1. Estático sin *chunk size* osea por defecto
Si solo escribes `schedule(static)`, OpenMP divide el número total de iteraciones en bloques aproximadamente iguales, uno para cada hilo.

Ejemplo: Si tienes 100 iteraciones y 4 hilos, OpenMP asignará las iteraciones 0 a 24 al Hilo 0, de la 25 a la 49 al Hilo 1, y así sucesivamente.

2. Estático con chunk size, puedes especificar el tamaño exacto del bloque usando schedule(static, n). Aquí, OpenMP divide las iteraciones en bloques de tamaño n y los reparte de forma circular (round-robin) entre los hilos.
Ejemplo: Si usas schedule(static, 10) con 100 iteraciones y 2 hilos:
Hilo 0 toma las iteraciones 0-9.
Hilo 1 toma las iteraciones 10-19.
Hilo 0 vuelve a tomar de la 20-29, y así sucesivamente.

| Característica | schedule(static) |
|----------------------------|---------------------|
| Caso de uso ideal | Cuando todas las iteraciones del bucle realizan exactamente la misma cantidad de trabajo y toman el mismo tiempo (por ejemplo, sumar elementos de dos arreglos gigantes).|
| Balance de carga | Malo si el trabajo es irregular. Si algunas iteraciones tardan mucho más en procesarse que otras, un hilo podría terminar su parte rápido y quedarse inactivo (idle), desperdiciando recursos mientras espera a que los demás terminen su asignación fija |
|Costo de gestión (Overhead)| Muy bajo. Como la división matemática se hace al principio, los hilos no pierden tiempo comunicándose entre sí para pedir más trabajo. Simplemente toman la parte que les tocó.|
---
## Schedule Dynamic


---
## Schedule Guided

---
## Conclusion


---
## Referencias
- TylerMSFT. (s. f.). D. Cláusula de programación. Microsoft Learn. https://learn.microsoft.com/es-es/cpp/parallel/openmp/d-using-the-schedule-clause?view=msvc-170
- Lücks. (2012, 1 junio). What’s the difference between «static» and «dynamic» schedule in OpenMP? Stack Overflow. https://stackoverflow.com/questions/10850155/whats-the-difference-between-static-and-dynamic-schedule-in-openmp

---
