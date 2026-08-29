/*
 * Kata 21: fseek y ftell
 *
 * Obtiene el tamano total del stream sin alterar su posicion actual. Comprueba
 * todos los errores de ftell y fseek. Recuerda restaurar la posicion incluso
 * despues de consultar el final.
 */

#include <stdio.h>

#include "exercise.h"

static int stream_size(FILE *stream, long *out_size) {
  long original_position;

  long size = -1L;

  if (stream == NULL || out_size == NULL) {
    return 0;
  }

  original_position = ftell(stream);
  if (original_position == -1L) {
    return 0;
  }

  if (fseek(stream, 0, SEEK_END) == 0) {
    size = ftell(stream);
  }

  if (fseek(stream, original_position, SEEK_SET) != 0) {
    return 0;
  }

  if (size == -1L) {
    return 0;
  }

  *out_size = size;

  return 1;
}

int main(void) {
  FILE *stream = tmpfile();
  long size = 0;
  long position;

  if (stream == NULL || fputs("0123456789", stream) == EOF ||
      fseek(stream, 3, SEEK_SET) != 0) {
    fprintf(stderr, "FAIL preparacion del stream\n");
    if (stream != NULL)
      fclose(stream);
    return 1;
  }
  if (!stream_size(stream, &size)) {
    fprintf(stderr, "FAIL stream_size\n");
    fclose(stream);
    return 1;
  }
  position = ftell(stream);
  if (size != 10 || position != 3) {
    fprintf(stderr, "FAIL size=%ld position=%ld\n", size, position);
    fclose(stream);
    return 1;
  }

  fclose(stream);
  return exercise_passed();
}
