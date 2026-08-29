/*
 * Kata 16: lectura incremental
 *
 * Lee stream hasta EOF y anade sus bytes al buffer. No asumas el tamano de la
 * entrada. Distingue el final normal del stream de un error de lectura.
 */

#include <stdint.h>
#include <stdio.h>
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
  buffer->capacity = 1;
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
  memcpy(buffer->data + buffer->length, text, text_length);
  buffer->length += text_length;
  buffer->data[buffer->length] = '\0';
  return 1;
}

static int buffer_read_stream(struct buffer *buffer, FILE *stream) {
  char chunk[1024];
  size_t bytes_read;

  while ((bytes_read = fread(chunk, 1, sizeof(chunk), stream)) > 0) {
    if (buffer_append(buffer, chunk, bytes_read) == 0) {
      return 0;
    }
  }

  if (ferror(stream)) {
    return 0;
  }

  return 1;
}

int main(void) {
  struct buffer buffer = {0};
  FILE *stream = tmpfile();
  const char input[] = "respuesta\nen streaming\n";

  if (stream == NULL || !buffer_init(&buffer)) {
    fprintf(stderr, "FAIL preparacion del test\n");
    if (stream != NULL)
      fclose(stream);
    return 1;
  }
  if (fwrite(input, 1, sizeof(input) - 1, stream) != sizeof(input) - 1 ||
      fseek(stream, 0, SEEK_SET) != 0) {
    fprintf(stderr, "FAIL escritura del stream de prueba\n");
    fclose(stream);
    buffer_destroy(&buffer);
    return 1;
  }

  if (!buffer_read_stream(&buffer, stream)) {
    fprintf(stderr, "FAIL buffer_read_stream\n");
    fclose(stream);
    buffer_destroy(&buffer);
    return 1;
  }
  if (buffer.length != sizeof(input) - 1 || strcmp(buffer.data, input) != 0) {
    fprintf(stderr, "FAIL contenido leido incorrecto\n");
    fclose(stream);
    buffer_destroy(&buffer);
    return 1;
  }

  fclose(stream);
  buffer_destroy(&buffer);
  return exercise_passed();
}
