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
### El Contexto: ¿Por qué necesitamos un "Schedule"?

Cuando paralelices un bucle usando la directiva `#pragma omp for`, OpenMP necesita saber qué hilo ejecutará qué iteraciones. Si tienes un bucle de 100 iteraciones y 4 hilos de procesamiento, no puedes dejar que todos hagan todo. Necesitas dividirlos. 

Aquí es donde entra el *scheduling* (planificación). Las dos estrategias principales son **estática** y **dinámica**, y la elección depende completamente de la naturaleza del trabajo dentro de tu bucle.

---
### Schedule Static


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
<img width="353" height="459" alt="image" src="https://github.com/user-attachments/assets/124a6acb-a0d2-48d6-83f7-7734aa264616" />

---
## Schedule Dynamic

### Funcionamiento de Schedule Dynamic

La cláusula schedule(dynamic) en OpenMP controla cómo se distribuyen las iteraciones de un bucle paralelo entre los hilos de ejecución (threads), realizando esta asignación sobre la marcha durante el tiempo de ejecución.

Imagina un bucle donde algunas iteraciones son baratas y otras caras. Con , un hilo puede quedarse atascado con muchas iteraciones pesadas mientras otros terminan pronto y se quedan inactivos. Con , los hilos de reposo pueden agarrar los trozos restantes y mantener la máquina ocupada.

### Balanceo de carga

El balanceo de carga, es muy eficiente para dynamic, ya que garantiza que cada hilo se mantenga ocupado, independientemente que tantas interaciones deben de realizar. Por ejemplo si un hilo tiene una gran catidad de iteraciones procesaran menos y las que tienen menos iteraciones procesaran una mayor cantidad, haciendo que no se desperdicie tiempo.

### Overhead

Debido a que requiere una sincrinizacion constante, esto hacer que el Overhead aumente, cuando el numero de hilos es mayor que la cantidad de chunks el overhead sera grande debido a que tiene esto satura pidiendo más tareas constantemente.

### En resumen

| Caracteristica | Schedule Dynamic |
|----------------|------------------|
|Caso de uso ideal| Iteraciones con costo muy irregular o impredecible (ray tracing, simulaciones con condicionales costosos, procesamiento de grafos)|
| Balance de carga | Óptimo — los hilos libres toman trabajo inmediatamente; la carga se equilibra de forma continua |
| Costo de gestion (overhead) | Alto — cada petición de chunk requiere una operación atómica sobre la cola compartida (~50–200 ns por petición) |

<img width="323" height="534" alt="image" src="https://github.com/user-attachments/assets/2f14ac6d-8504-403d-bc6e-51396f9a63c8" />


---
## Schedule Guided
La planificación guiada es dinámica, pero a diferencia de dynamic (donde el tamaño del bloque es siempre fijo), en guided el tamaño del bloque disminuye de forma exponencial conforme avanza el bucle.

Comienza repartiendo pedazos muy grandes de iteraciones. Con esto, se ahorra mucho tiempo de comunicación entre los hilos y el planificador. A medida que quedan menos iteraciones por procesar, los bloques que reparte se vuelven cada vez más pequeños. Esto asegura que al final del bucle, cuando los hilos están por terminar, no haya un hilo atascado con un bloque gigante mientras los demás están inactivos.

Funciona de la siguiente manera, dependiendo del parámetro chunk size (tamaño de bloque mínimo):

 1. Guiado sin chunk size (por defecto): Si solo escribes schedule(guided), OpenMP calcula el tamaño de los bloques dividiendo las iteraciones restantes entre el número total de hilos. El tamaño del bloque irá disminuyendo hasta llegar a 1 (el mínimo por defecto).

Ejemplo: Con 100 iteraciones y 4 hilos, el Hilo 0 podría recibir las primeras 25 iteraciones, luego el Hilo 1 recibe 19, el Hilo 2 recibe 14... y así progresivamente hasta agotar las iteraciones.
 
 2. Guiado con chunk size: Si usas schedule(guided, n), OpenMP hace la misma división, pero garantiza que el bloque nunca será menor que n (excepto, posiblemente, en el último bloque si quedan menos de n iteraciones).

Ejemplo: Con schedule(guided, 5) y 100 iteraciones, los bloques se reducirán progresivamente pero nunca bajarán de 5 iteraciones por asignación.

| Característica | schedule(guided) |
|----------------------------|---------------------|
| Caso de uso ideal | Cuando el trabajo por iteración es desigual o impredecible, pero hay tantas iteraciones que usar dynamic generaría demasiada lentitud por la administración (overhead).|
| Balance de carga | 	Mejor que static para trabajo irregular. Los bloques grandes al inicio reducen el overhead de sincronización, y los bloques pequeños al final permiten un balance fino entre hilos. |
|Costo de gestión (Overhead)| Medio. Es más alto que el estático (porque hay que calcular y asignar sobre la marcha), pero mucho menor que el dinámico (porque se hacen muchas menos entregas gracias a los bloques grandes del inicio).|

<img width="447" height="642" alt="image" src="https://github.com/user-attachments/assets/e45c93bc-6c6f-4b92-9928-d1486f169b22" />

---
## Conclusion


---
## Referencias
- TylerMSFT. (s. f.). D. Cláusula de programación. Microsoft Learn. https://learn.microsoft.com/es-es/cpp/parallel/openmp/d-using-the-schedule-clause?view=msvc-170
- Lücks. (2012, 1 junio). What’s the difference between «static» and «dynamic» schedule in OpenMP? Stack Overflow. https://stackoverflow.com/questions/10850155/whats-the-difference-between-static-and-dynamic-schedule-in-openmp
- CRONOGRAMA. (s.f.). Schedule https://www.cenapad.unicamp.br/parque/manuais/Xlf/lr218.HTM
- OpenMP - Programación (estática, dinámica, guiada, en tiempo de ejecución, automática) - Zona tecnológica de Yiling. (2020, July 15). https://610yilingliu.github.io/2020/07/15/ScheduleinOpenMP/
- OpenMP Scheduling. (s.f.). Ian Finlayson. https://ianfinlayson.net/class/cpsc425/notes/12-scheduling
- Yiling. (2020, 15 de julio). OpenMP - Scheduling(static, dynamic, guided, runtime, auto) - Yiling's Tech Zone | 风逝无殇的瞎逼逼基地. Yiling's Tech Zone | 风逝无殇的瞎逼逼基地. https://610yilingliu.github.io/2020/07/15/ScheduleinOpenMP/
- Sobral, J. B. M. (s.f.). OpenMP Scheduling [Archivo PDF]. Universidade Federal de Santa Catarina. http://www.inf.ufsc.br/~bosco/ensino/ine5645/OpenMP_Dynamic_Scheduling.pdf
---
