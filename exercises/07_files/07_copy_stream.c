/*
 * Kata 23: copia por bloques
 *
 * Copia source en destination sin asumir su tamano. Usa un array temporal,
 * fread y fwrite. Maneja lecturas parciales, errores y escrituras que procesen
 * menos bytes que los solicitados. No cierres ninguno de los streams.
 */

#include <stdio.h>
#include <string.h>

#include "exercise.h"

static int stream_copy(FILE *source, FILE *destination) {
  unsigned char buffer[1024];
  size_t bytes_read;

  while ((bytes_read = fread(buffer, 1, sizeof(buffer), source)) > 0) {
    size_t total_written = 0;

    while (total_written < bytes_read) {
      size_t written = fwrite(buffer + total_written, 1,
                              bytes_read - total_written, destination);

      if (written == 0) {
        return 0;
      }

      total_written += written;
    }
  }

  if (ferror(source)) {
    return 0;
  }

  return 1;
}

int main(void) {
  const char input[] = "chunk 1\nchunk 2\nfin\n";
  char output[64] = {0};
  FILE *source = tmpfile();
  FILE *destination = tmpfile();
  size_t bytes_read;

  if (source == NULL || destination == NULL ||
      fwrite(input, 1, sizeof(input) - 1, source) != sizeof(input) - 1 ||
      fseek(source, 0, SEEK_SET) != 0) {
    fprintf(stderr, "FAIL preparacion de streams\n");
    if (source != NULL)
      fclose(source);
    if (destination != NULL)
      fclose(destination);
    return 1;
  }
  if (!stream_copy(source, destination) || fflush(destination) == EOF ||
      fseek(destination, 0, SEEK_SET) != 0) {
    fprintf(stderr, "FAIL stream_copy\n");
    fclose(source);
    fclose(destination);
    return 1;
  }
  bytes_read = fread(output, 1, sizeof(output) - 1, destination);
  output[bytes_read] = '\0';
  if (bytes_read != sizeof(input) - 1 || strcmp(output, input) != 0) {
    fprintf(stderr, "FAIL contenido copiado incorrecto\n");
    fclose(source);
    fclose(destination);
    return 1;
  }

  fclose(source);
  fclose(destination);
  return exercise_passed();
}
