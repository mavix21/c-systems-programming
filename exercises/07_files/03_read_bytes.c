/*
 * Kata 19: fgetc y EOF
 *
 * Cuenta los bytes restantes del stream desde su posicion actual. Usa int para
 * recibir fgetc: debe poder representar todos los unsigned char y tambien EOF.
 * Retorna 0 ante un error y escribe el resultado en out_count al completar.
 */

#include <stdio.h>

#include "exercise.h"

static int stream_count_bytes(FILE *stream, size_t *out_count) {
  size_t count = 0;
  int character;

  while ((character = fgetc(stream)) != EOF) {
    count++;
  }

  if (ferror(stream)) {
    return 0;
  }

  *out_count = count;

  return 1;
}

int main(void) {
  FILE *stream = tmpfile();
  size_t count = 0;

  if (stream == NULL) {
    fprintf(stderr, "FAIL tmpfile\n");
    return 1;
  }
  if (fputs("abc\n123", stream) == EOF || fseek(stream, 2, SEEK_SET) != 0) {
    fprintf(stderr, "FAIL preparacion del stream\n");
    fclose(stream);
    return 1;
  }
  if (!stream_count_bytes(stream, &count)) {
    fprintf(stderr, "FAIL stream_count_bytes\n");
    fclose(stream);
    return 1;
  }
  if (count != 5) {
    fprintf(stderr, "FAIL esperados 5 bytes, obtenidos %zu\n", count);
    fclose(stream);
    return 1;
  }

  fclose(stream);
  return exercise_passed();
}
