/*
 * Kata 12: ciclo de vida de un buffer
 *
 * Inicializa un buffer vacio con initial_capacity bytes. La capacidad incluye
 * el byte nulo que mantiene data como string de C. buffer_destroy debe aceptar
 * un buffer vacio, liberar sus recursos y reiniciar todos sus campos.
 */

#include <stdlib.h>

#include "exercise.h"

struct buffer {
  char *data;
  size_t length;
  size_t capacity;
};

static void buffer_destroy(struct buffer *buffer) {
  if (buffer == NULL)
    return;

  free(buffer->data);
  *buffer = (struct buffer){0};
}

static int buffer_init(struct buffer *buffer, size_t initial_capacity) {
  char *data;

  if (buffer == NULL)
    return 0;

  *buffer = (struct buffer){0};
  if (initial_capacity == 0)
    return 1;

  if (initial_capacity > SIZE_MAX)
    return 0;

  data = calloc(initial_capacity, sizeof(*data));
  if (data == NULL)
    return 0;

  buffer->capacity = initial_capacity;
  buffer->length = 0;
  buffer->data = data;

  return 1;
}

int main(void) {
  struct buffer buffer = {0};

  if (!buffer_init(&buffer, 16)) {
    fprintf(stderr, "FAIL buffer_init no pudo crear el buffer\n");
    return 1;
  }
  if (buffer.data == NULL || buffer.length != 0 || buffer.capacity != 16) {
    fprintf(stderr, "FAIL estado inicial incorrecto\n");
    buffer_destroy(&buffer);
    return 1;
  }
  if (buffer.data[0] != '\0') {
    fprintf(stderr, "FAIL el buffer no comienza como string vacia\n");
    buffer_destroy(&buffer);
    return 1;
  }

  buffer_destroy(&buffer);
  if (buffer.data != NULL || buffer.length != 0 || buffer.capacity != 0) {
    fprintf(stderr, "FAIL buffer_destroy no reinicio el estado\n");
    return 1;
  }

  return exercise_passed();
}
