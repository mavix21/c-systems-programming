/*
 * Kata 20: fgets y estados de lectura
 *
 * Lee una linea en output y elimina su '\n' final si existe. Retorna READ_OK,
 * READ_EOF o READ_ERROR. Para esta kata puedes asumir que la linea cabe entera
 * en output, incluyendo el byte nulo.
 */

#include <stdio.h>
#include <string.h>

#include "exercise.h"

enum read_status { READ_ERROR = -1, READ_EOF = 0, READ_OK = 1 };

static enum read_status stream_read_line(FILE *stream, char *output,
                                         size_t output_size) {
  size_t length;

  if (fgets(output, output_size, stream) == NULL) {
    if (ferror(stream)) {
      return READ_ERROR;
    }

    return READ_EOF;
  }

  length = strlen(output);
  if (length > 0 && output[length - 1] == '\n') {
    output[length - 1] = '\0';
  }

  return READ_OK;
}

int main(void) {
  FILE *stream = tmpfile();
  char line[16];

  if (stream == NULL || fputs("first\nsecond\n", stream) == EOF ||
      fseek(stream, 0, SEEK_SET) != 0) {
    fprintf(stderr, "FAIL preparacion del stream\n");
    if (stream != NULL)
      fclose(stream);
    return 1;
  }
  if (stream_read_line(stream, line, sizeof(line)) != READ_OK ||
      strcmp(line, "first") != 0) {
    fprintf(stderr, "FAIL primera linea\n");
    fclose(stream);
    return 1;
  }
  if (stream_read_line(stream, line, sizeof(line)) != READ_OK ||
      strcmp(line, "second") != 0) {
    fprintf(stderr, "FAIL segunda linea\n");
    fclose(stream);
    return 1;
  }
  if (stream_read_line(stream, line, sizeof(line)) != READ_EOF) {
    fprintf(stderr, "FAIL se esperaba READ_EOF\n");
    fclose(stream);
    return 1;
  }

  fclose(stream);
  return exercise_passed();
}
