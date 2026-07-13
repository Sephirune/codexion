*Este proyecto ha sido creado como parte del currículo de 42 por aarogarc.*

# Codexion

## Descripción

Codexion es una simulación de concurrencia basada en el problema clásico de los
filósofos cenando. Varias personas que programan (hilos) se sientan en un hub
circular y alternan entre tres estados: compilar, depurar y refactorizar. Para
compilar necesitan dos dongles USB simultáneamente (el de su izquierda y el de
su derecha), y hay tantos dongles como coders, por lo que dos coders adyacentes
compiten siempre por el dongle que comparten.

Si un coder pasa más de `time_to_burnout` milisegundos sin empezar a compilar,
se agota y la simulación termina. El objetivo es coordinar el acceso a los
dongles con hilos POSIX, mutexes y variables de condición, evitando
interbloqueos e inanición, y detectando el agotamiento con una precisión
inferior a 10 ms.

El programa soporta dos políticas de arbitraje: `fifo` (orden de llegada) y
`edf` (Earliest Deadline First, prioridad para el coder más cercano a agotarse).

## Instrucciones

Compilación (usa `-Wall -Wextra -Werror -pthread`):

```
make
```

Ejecución:

```
./codexion number_of_coders time_to_burnout time_to_compile time_to_debug \
           time_to_refactor number_of_compiles_required dongle_cooldown scheduler
```

Todos los argumentos son obligatorios. Los siete primeros deben ser enteros
positivos y el scheduler debe ser exactamente `fifo` o `edf`.

Ejemplos:

```
./codexion 5 800 200 200 200 3 50 fifo     # caso general
./codexion 3 900 200 100 100 5 0 edf       # EDF, nadie debe agotarse
./codexion 1 800 200 200 200 3 0 fifo      # un solo coder, un solo dongle
```

La simulación termina cuando alguien se agota o cuando todos los coders han
compilado al menos `number_of_compiles_required` veces.

## Blocking cases handled

- **Prevención de interbloqueos (condiciones de Coffman)**: el deadlock
  circular requiere que todos los coders retengan un dongle mientras esperan
  el del vecino. Rompo la condición de espera circular imponiendo un orden de
  adquisición asimétrico: los coders con id impar toman primero el dongle de
  menor índice y los pares el de mayor índice. Así dos vecinos nunca pueden
  bloquearse mutuamente y siempre hay al menos un coder que avanza. Además,
  este reparto reduce la contención frente a un orden global único.

- **Prevención de inanición**: cada dongle tiene una cola de espera propia.
  Con `fifo` se respeta estrictamente el orden de llegada; con `edf` la cola
  se mantiene ordenada por deadline (`last_compile_start + time_to_burnout`)
  y en caso de empate gana el id menor, lo que hace el desempate determinista.
  Un coder solo toma el dongle si es el primero de la cola, aunque el dongle
  esté libre.

- **Gestión del cooldown**: al liberarse, cada dongle guarda su timestamp de
  liberación. Si al coder le toca el turno pero el cooldown no ha expirado,
  espera con `pthread_cond_timedwait` exactamente el tiempo restante,
  liberando el mutex mientras duerme para no bloquear al resto.

- **Detección precisa del agotamiento**: un hilo monitor independiente
  muestrea el estado de todos los coders cada 1 ms, de modo que el desfase
  máximo de detección queda muy por debajo de los 10 ms exigidos. Un coder
  que ya cumplió su cuota de compilaciones queda excluido de la vigilancia.

- **Serialización del log**: todas las escrituras pasan por un único
  `log_mutex`, por lo que nunca se mezclan dos mensajes en la misma línea y
  los timestamps salen siempre en orden. Tras el agotamiento no se imprime
  ningún mensaje más.

- **Parada limpia**: al terminar, el monitor hace broadcast sobre la condvar
  de cada dongle para despertar a los coders que pudieran estar bloqueados
  esperando un recurso; al despertar comprueban el flag de parada y salen,
  de modo que todos los `pthread_join` retornan y no queda ningún hilo
  colgado ni memoria sin liberar.

## Thread synchronization mechanisms

- **pthread_mutex_t**: hay un mutex por dongle, que protege todo su estado
  (disponibilidad, timestamp de liberación y cola de espera), y un mutex
  global de log. La granularidad por dongle permite que operaciones sobre
  dongles distintos ocurran en paralelo.

- **pthread_cond_t**: cada dongle tiene una variable de condición donde
  duermen los coders que lo esperan, sin consumir CPU. La espera se hace
  siempre dentro de un bucle `while` que re-evalúa la condición (dongle libre
  y ser el primero de la cola), lo que protege contra spurious wakeups y
  contra el hecho de que `pthread_cond_broadcast` despierta a todos los
  hilos: solo el que realmente tiene el turno continúa, el resto vuelve a
  dormir. Se usa broadcast en lugar de signal porque cada coder tiene una
  condición de avance distinta y solo él puede evaluarla.

- **Comunicación thread-safe coders/monitor**: el estado compartido de
  parada es un `volatile int stop` dentro de la estructura de la simulación,
  que el monitor escribe y los coders leen en cada fase y dentro de los
  sleeps (troceados en pasos de 100 us para poder interrumpirse). El caso
  crítico es el mensaje de agotamiento: el monitor imprime "burned out" y
  pone `stop = 1` dentro del mismo bloqueo de `log_mutex` que usan los
  coders para imprimir. Esto crea una frontera atómica: cualquier mensaje
  que entre al mutex después del monitor ve el flag activado y no se
  imprime, evitando la condición de carrera en la que el aviso de burnout
  podría perderse o aparecer mezclado con logs posteriores.

- **Evitación de condiciones de carrera en la adquisición**: encolarse,
  esperar el turno, salir de la cola y marcar el dongle como ocupado ocurre
  como una única transacción bajo el mutex del dongle, de forma que ningún
  otro coder puede colarse entre la comprobación del turno y la toma del
  recurso.

## Recursos

- man pages: pthread_create(3), pthread_mutex_lock(3), pthread_cond_wait(3),
  pthread_cond_timedwait(3), gettimeofday(2)
- E. W. Dijkstra, "Hierarchical ordering of sequential processes" (origen del
  problema de los filósofos cenando)
- Condiciones de Coffman: https://en.wikipedia.org/wiki/Deadlock
- Earliest Deadline First scheduling:
  https://en.wikipedia.org/wiki/Earliest_deadline_first_scheduling

**Uso de IA**: he utilizado IA (Claude) como apoyo durante el desarrollo,
principalmente para revisar la lógica de concurrencia, detectar y depurar
errores (un bug en la comparación del primer elemento de la cola de espera,
el reinicio del temporizador de burnout tras el delay inicial y fugas de
memoria en la limpieza final), proponer parámetros de prueba y verificar el
cumplimiento de la norma. Todo el código entregado lo he revisado, probado y
soy capaz de explicarlo y modificarlo.
