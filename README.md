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
# Schedule Static
# El Contexto: ¿Por qué necesitamos un "Schedule"?

Cuando paralelices un bucle usando la directiva `#pragma omp for`, OpenMP necesita saber qué hilo ejecutará qué iteraciones. Si tienes un bucle de 100 iteraciones y 4 hilos de procesamiento, no puedes dejar que todos hagan todo. Necesitas dividirlos. 

Aquí es donde entra el *scheduling* (planificación). Las dos estrategias principales son **estática** y **dinámica**, y la elección depende completamente de la naturaleza del trabajo dentro de tu bucle.

# La Teoría detrás de `schedule(static)`

Como su nombre lo indica, la planificación estática es **predeterminada y determinista**. La repartición del trabajo se calcula y se asigna a los hilos *antes* de que el bucle comience a ejecutarse. 

Funciona de la siguiente manera, dependiendo de si especificas o no un parámetro adicional llamado *chunk size* (tamaño de bloque):

 1. Estático sin *chunk size* (Por defecto)
Si solo escribes `schedule(static)`, OpenMP divide el número total de iteraciones en bloques aproximadamente iguales, uno para cada hilo.

* **Ejemplo:** Si tienes 100 iteraciones y 4 hilos, OpenMP asignará las iteraciones 0 a 24 al Hilo 0, de la 25 a la 49 al Hilo 1, y así sucesivamente.


---
## Schedule Dynamic


---
## Schedule Guided

---
## Conclusion


---
## Referencias

---
