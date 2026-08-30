# C Playground

Katas progresivas para recuperar C y avanzar hacia la construccion de una
aplicacion de terminal interactiva. Los ejercicios son pequenos, tienen sus
pruebas en el mismo archivo y estan pensados para resolverse en orden.

El plan completo desde las primitivas hasta el cliente de agente se encuentra
en [`ROADMAP.md`](ROADMAP.md).

## Toolchain

Este proyecto usa C17 y compila con advertencias estrictas:

```text
-std=c17 -Wall -Wextra -Werror -pedantic -g
```

En este equipo estan disponibles:

- Apple Clang 21 (`clang` y `gcc` apuntan al mismo compilador)
- GNU Make 3.81
- LLDB 21
- AddressSanitizer y UndefinedBehaviorSanitizer mediante Clang
- `leaks`, incluido en macOS

`valgrind` no esta instalado. En macOS moderno se recomienda empezar con los
sanitizers y usar `leaks` como comprobacion adicional.

## Primeros pasos

```sh
make list
make run EXERCISE=00_fundamentals/01_temperature
```

La primera ejecucion debe fallar: esa es la kata. Abre el archivo indicado,
busca `TODO` e implementa solo la funcion del ejercicio. Vuelve a ejecutar el
mismo comando hasta ver `PASS`.

Comandos utiles:

```sh
make build EXERCISE=01_pointers/01_swap
make run EXERCISE=01_pointers/01_swap
make sanitize EXERCISE=05_memory/01_range
make debug EXERCISE=01_pointers/01_swap
make leaks EXERCISE=05_memory/01_range
make verify
make clean
```

`make verify` solo comprueba que todas las katas compilan. No espera que esten
resueltas.

## Metodo de trabajo

1. Lee el enunciado al principio del archivo.
2. Predice el resultado antes de compilar.
3. Implementa la funcion marcada con `TODO` sin modificar las pruebas.
4. Compila y ejecuta con `make run EXERCISE=...`.
5. Para ejercicios con memoria, repite con `make sanitize EXERCISE=...`.
6. Si algo no se entiende, inspecciona variables y memoria con LLDB.

## Ruta de aprendizaje

Las primeras katas cubren fundamentos, punteros, arrays, strings, structs y
memoria dinamica. La ruta crecera en estas etapas:

1. Fundamentos, funciones y representacion de datos
2. Punteros, arrays y strings
3. Structs, enums y diseno de APIs pequenas
4. Memoria dinamica y ownership manual
5. Listas enlazadas, pilas, colas y buffers
6. Archivos, streams y manejo de errores
7. Procesos, pipes y senales POSIX
8. Terminales, modo raw, ANSI y lectura de teclado
9. Event loop, historial, comandos y renderizado incremental
10. Proyecto final: asistente de terminal minimalista

## Bloque de memoria

Despues de completar las seis katas iniciales, continua en este orden:

```text
05_memory/02_zeroed_array
05_memory/03_duplicate_string
05_memory/04_grow_array
05_memory/05_filter_positive
05_memory/06_matrix
```

Ejecuta siempre estas katas con `make sanitize EXERCISE=...`. Que una prueba
muestre `PASS` no garantiza por si solo que no existan escrituras fuera de
limites, uso de memoria liberada o fugas.

La variante resuelta de la matriz que utiliza un unico bloque contiguo esta en
`examples/05_memory/06_matrix_contiguous.c`. Se ejecuta con:

```sh
make run-example EXAMPLE=05_memory/06_matrix_contiguous
```

## Bloque de buffers

El siguiente bloque construye la primitiva que usaremos para entrada, archivos
y respuestas en streaming:

```text
06_buffers/01_lifecycle
06_buffers/02_reserve
06_buffers/03_push
06_buffers/04_append
06_buffers/05_read_stream
```

## Bloque de archivos

Este bloque introduce `FILE *` desde streams temporales hasta copia robusta por
bloques:

```text
07_files/01_temporary_stream
07_files/02_open_modes
07_files/03_read_bytes
07_files/04_read_lines
07_files/05_seek_and_tell
07_files/06_binary_io
07_files/07_copy_stream
```

Empieza con:

```sh
make sanitize EXERCISE=07_files/01_temporary_stream
```

## Bloque de colecciones

Este bloque generaliza el buffer dinamico hasta llegar al historial del chat:

```text
08_collections/01_vector_lifecycle
08_collections/02_vector_reserve
08_collections/03_push_pop
08_collections/04_insert
08_collections/05_remove
08_collections/06_generic_vector
08_collections/07_owned_strings
08_collections/08_message_list
```

Empieza con:

```sh
make sanitize EXERCISE=08_collections/01_vector_lifecycle
```

## Bloque de listas y colas

Estas katas introducen estructuras enlazadas y almacenamiento circular:

```text
09_queues/01_linked_push_front
09_queues/02_linked_append
09_queues/03_linked_remove
09_queues/04_fifo_queue
09_queues/05_ring_buffer
09_queues/06_command_history
```

Empieza con:

```sh
make sanitize EXERCISE=09_queues/01_linked_push_front
```

## C17 frente al C de Holberton/ALX

Muchos proyectos historicos de Holberton/ALX usan GNU89 y reglas de estilo
Betty. Aqui mantenemos su enfoque de ejercicios y compilacion estricta, pero
usamos C17: es una base mas apropiada para escribir hoy una aplicacion nueva.
Cuando sea pedagogicamente util, una kata indicara diferencias con C89.
