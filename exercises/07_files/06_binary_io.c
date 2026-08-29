/*
 * Kata 22: fread y fwrite binarios
 *
 * Escribe y lee arrays de uint32_t comprobando que se procesen todos sus
 * elementos. Este formato solo se usa para practicar localmente: escribir la
 * representacion cruda no es portable entre arquitecturas con distinto endian.
 */

#include <stdint.h>
#include <stdio.h>

#include "exercise.h"

static int write_values(FILE *stream, const uint32_t *values, size_t count) {
  size_t written = fwrite(values, sizeof(*values), count, stream);

  return written == count;
}

static int read_values(FILE *stream, uint32_t *values, size_t count) {
  size_t read = fread(values, sizeof(*values), count, stream);

  return read == count;
}

int main(void) {
  const uint32_t input[] = {10, 20, 30, 4000000000U};
  uint32_t output[4] = {0};
  FILE *stream = tmpfile();

  if (stream == NULL) {
    fprintf(stderr, "FAIL tmpfile\n");
    return 1;
  }
  if (!write_values(stream, input, sizeof(input) / sizeof(input[0])) ||
      fseek(stream, 0, SEEK_SET) != 0 ||
      !read_values(stream, output, sizeof(output) / sizeof(output[0]))) {
    fprintf(stderr, "FAIL binary I/O\n");
    fclose(stream);
    return 1;
  }
  for (size_t i = 0; i < sizeof(input) / sizeof(input[0]); i++) {
    if (output[i] != input[i]) {
      fprintf(stderr, "FAIL valor binario en indice %zu\n", i);
      fclose(stream);
      return 1;
    }
  }

  fclose(stream);
  return exercise_passed();
}
