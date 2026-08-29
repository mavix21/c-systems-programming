/*
 * Kata 13: capacidad y realloc
 *
 * Asegura que el buffer tenga espacio para additional bytes nuevos y el byte
 * nulo final. Si ya hay espacio, no hagas nada. Si realloc falla, conserva
 * intactos el puntero, el contenido y la capacidad anteriores.
 */

#include <stdint.h>
#include <stdlib.h>
#include <string.h>

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
  char *data = calloc(initial_capacity, sizeof(*data));

  if (data == NULL)
    return 0;
  buffer->data = data;
  buffer->length = 0;
  buffer->capacity = initial_capacity;
  return 1;
}

static int buffer_reserve(struct buffer *buffer, size_t additional) {
  if (buffer == NULL)
    return 0;

  if (buffer->length == SIZE_MAX || additional > SIZE_MAX - buffer->length - 1)
    return 0;

  // Extra byte needed for null character
  // (it is a char* buffer treated as a string)
  size_t new_required_capacity = buffer->length + additional + 1;

  if (new_required_capacity <= buffer->capacity) {
    return 1;
  }

  size_t new_capacity = buffer->capacity == 0 ? 8 : buffer->capacity;
  while (new_capacity < new_required_capacity) {
    if (new_capacity > SIZE_MAX / 2) {
      new_capacity = new_required_capacity;
      break;
    }

    new_capacity *= 2;
  }

  char *new_data = realloc(buffer->data, new_capacity * sizeof(*buffer->data));
  if (new_data == NULL) {
    return 0;
  }

  buffer->data = new_data;
  buffer->capacity = new_capacity;

  return 1;
}

int main(void) {
  struct buffer buffer = {0};

  if (!buffer_init(&buffer, 4)) {
    fprintf(stderr, "FAIL buffer_init\n");
    return 1;
  }
  buffer.data[0] = 'h';
  buffer.data[1] = 'i';
  buffer.data[2] = '\0';
  buffer.length = 2;

  if (!buffer_reserve(&buffer, 20)) {
    fprintf(stderr, "FAIL buffer_reserve\n");
    buffer_destroy(&buffer);
    return 1;
  }
  if (buffer.capacity < buffer.length + 20 + 1) {
    fprintf(stderr, "FAIL capacidad insuficiente\n");
    buffer_destroy(&buffer);
    return 1;
  }
  if (strcmp(buffer.data, "hi") != 0 || buffer.length != 2) {
    fprintf(stderr, "FAIL no se conservo el contenido\n");
    buffer_destroy(&buffer);
    return 1;
  }

  buffer_destroy(&buffer);
  return exercise_passed();
}
