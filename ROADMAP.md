# Roadmap hacia un agente de terminal

El objetivo no es saltar directamente a una TUI, sino construir y entender sus
primitivas en capas. Cada bloque debe poder probarse sin depender de los bloques
posteriores.

## 1. Lenguaje y memoria

Estado: completado.

- Tipos, funciones, punteros, arrays y strings
- Structs y contratos de funciones
- `malloc`, `calloc`, `realloc` y `free`
- Ownership, cleanup parcial y sanitizers
- Matrices y representaciones de memoria

## 2. Buffers dinamicos

Estado: completado.

- Ciclo de vida de un buffer
- Capacidad frente a longitud
- Crecimiento geometrico
- Append de bytes y strings
- Lectura incremental de streams

Aplicacion final: entrada del usuario, tokens recibidos por streaming,
construccion de prompts y lectura de archivos.

## 3. Colecciones

Estado: en curso.

- Vector dinamico generico
- Lista de mensajes
- Cola y ring buffer
- Hash table de strings
- Cache LRU pequena
- Iteradores y callbacks

Aplicacion final: historial, mensajes, comandos recientes y cache.

Progresion completada: vector de enteros, insercion y eliminacion, vector
generico, ownership profundo de strings, lista de mensajes, listas enlazadas,
cola FIFO, ring buffer e historial circular de comandos.

La progresion actual cubre un registro hash de herramientas, callbacks con
contexto y una cache LRU de archivos del workspace. Estas primitivas preparan
el registro de tools, la ayuda de comandos y la cache local del agente.

## 4. Archivos y configuracion

- `FILE *`, modos, posicion y estados de error
- Lectura y escritura de texto y datos binarios
- Copia incremental de streams
- Rutas y directorios
- Lectura y escritura robustas
- Escritura atomica con archivo temporal
- Formato key-value pequeno
- Parser y serializer JSON
- Descubrimiento y carga de archivos de skills

Aplicacion final: configuracion, sesiones, cache y skills en Markdown.

## 5. Unix y procesos

- Descriptores de archivo
- `read` y `write`
- Pipes
- Creacion y espera de procesos
- Captura de stdout y stderr
- Senales y cancelacion

Aplicacion final: ejecutar herramientas locales y cancelar operaciones.

## 6. Terminal

- Secuencias ANSI
- Dimensiones del terminal
- Modo canonico y modo raw
- Lectura de teclas y secuencias de escape
- Editor de linea y cursor
- Renderizado incremental
- Resize, scroll e historial

Aplicacion final: interfaz interactiva sin depender inicialmente de ncurses.

## 7. Event loop

- I/O no bloqueante
- `poll`
- Timers
- Estado y eventos
- Backpressure
- Integracion de teclado, red y procesos

Aplicacion final: mantener la interfaz activa mientras llegan tokens.

## 8. Cliente de agente

- URL y headers HTTP
- Uso de una libreria TLS/HTTP del sistema
- Requests JSON
- Respuestas en streaming SSE
- Modelo de mensajes y tool calls
- Cancelacion, reintentos y errores

Aplicacion final: conversacion real con un modelo. Implementaremos la logica del
cliente; no intentaremos reimplementar criptografia TLS.

## 9. Producto minimo

- Prompt interactivo
- Streaming de respuesta
- Historial persistente
- Cache local
- Carga de skills
- Herramientas con confirmacion
- Configuracion y logs

Cada etapa tendra katas pequenas antes de integrarse en el programa final.
