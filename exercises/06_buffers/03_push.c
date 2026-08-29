/*
 * Kata 14: insertar un byte
 *
 * Anade un caracter al final, aumenta length y conserva el byte nulo final.
 * buffer_reserve ya resuelve el crecimiento geometrico y los overflows.
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
  buffer->capacity = initial_capacity;
  buffer->length = 0;
  return 1;
}

static int buffer_reserve(struct buffer *buffer, size_t additional) {
  size_t required;
  size_t new_capacity;
  char *resized;

  if (additional > SIZE_MAX - buffer->length - 1)
    return 0;
  required = buffer->length + additional + 1;
  if (required <= buffer->capacity)
    return 1;

  new_capacity = buffer->capacity == 0 ? 8 : buffer->capacity;
  while (new_capacity < required) {
    if (new_capacity > SIZE_MAX / 2) {
      new_capacity = required;
      break;
    }
    new_capacity *= 2;
  }

  resized = realloc(buffer->data, new_capacity * sizeof(*resized));
  if (resized == NULL)
    return 0;
  buffer->data = resized;
  buffer->capacity = new_capacity;
  return 1;
}

static int buffer_push(struct buffer *buffer, char value) {
  if (!buffer_reserve(buffer, 1))
    return 0;

  buffer->data[buffer->length] = value;
  buffer->data[buffer->length + 1] = '\0';
  buffer->length++;

  return 1;
}

int main(void) {
  struct buffer buffer = {0};

  if (!buffer_init(&buffer, 2)) {
    fprintf(stderr, "FAIL buffer_init\n");
    return 1;
  }
  if (!buffer_push(&buffer, 'A') || !buffer_push(&buffer, 'B') ||
      !buffer_push(&buffer, 'C')) {
    fprintf(stderr, "FAIL buffer_push\n");
    buffer_destroy(&buffer);
    return 1;
  }
  if (buffer.length != 3 || strcmp(buffer.data, "ABC") != 0) {
    fprintf(stderr, "FAIL contenido esperado ABC\n");
    buffer_destroy(&buffer);
    return 1;
  }

  buffer_destroy(&buffer);
  return exercise_passed();
}
