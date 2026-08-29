/*
 * Kata 17: primer FILE pointer
 *
 * Escribe text en un stream ya abierto. Comprueba los errores de fputs y
 * fflush. No cierres el stream: pertenece a quien llamo a la funcion.
 */

#include <stdio.h>
#include <string.h>

#include "exercise.h"

static int stream_write_text(FILE *stream, const char *text) {
  if (fputs(text, stream) == EOF) {
    return 0;
  }

  if (fflush(stream) == EOF) {
    return 0;
  }

  return 1;
}

int main(void) {
  FILE *stream = tmpfile();
  char output[32];

  if (stream == NULL) {
    fprintf(stderr, "FAIL tmpfile\n");
    return 1;
  }
  if (!stream_write_text(stream, "hola, archivo")) {
    fprintf(stderr, "FAIL stream_write_text\n");
    fclose(stream);
    return 1;
  }
  rewind(stream);
  if (fgets(output, sizeof(output), stream) == NULL ||
      strcmp(output, "hola, archivo") != 0) {
    fprintf(stderr, "FAIL contenido escrito incorrecto\n");
    fclose(stream);
    return 1;
  }

  fclose(stream);
  return exercise_passed();
}
