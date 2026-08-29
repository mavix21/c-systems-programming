/*
 * Kata 15: append de un bloque
 *
 * Anade text_length bytes de text con una sola reserva. No uses strcat: el
 * parametro text no tiene por que terminar en byte nulo.
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

static int buffer_init(struct buffer *buffer) {
  char *data = calloc(1, sizeof(*data));

  if (data == NULL)
    return 0;
  buffer->data = data;
  buffer->length = 0;
  buffer->capacity = 1;
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

  new_capacity = buffer->capacity;
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

static int buffer_append(struct buffer *buffer, const char *text,
                         size_t text_length) {
  if (!buffer_reserve(buffer, text_length))
    return 0;

  for (size_t i = 0; i < text_length; i++) {
    buffer->data[buffer->length + i] = text[i];
  }

  buffer->length += text_length;
  buffer->data[buffer->length] = '\0';

  return 1;
}

int main(void) {
  struct buffer buffer = {0};
  const char first[] = {'h', 'o', 'l', 'a'};

  if (!buffer_init(&buffer)) {
    fprintf(stderr, "FAIL buffer_init\n");
    return 1;
  }
  if (!buffer_append(&buffer, first, sizeof(first)) ||
      !buffer_append(&buffer, ", C", 3) ||
      !buffer_append(&buffer, "ignorado", 0)) {
    fprintf(stderr, "FAIL buffer_append\n");
    buffer_destroy(&buffer);
    return 1;
  }
  if (buffer.length != 7 || strcmp(buffer.data, "hola, C") != 0) {
    fprintf(stderr, "FAIL contenido esperado: hola, C\n");
    buffer_destroy(&buffer);
    return 1;
  }

  buffer_destroy(&buffer);
  return exercise_passed();
}
