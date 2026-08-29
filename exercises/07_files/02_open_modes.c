/*
 * Kata 18: fopen, modos y ownership
 *
 * Abre path en modo escritura, guarda text y cierra el archivo. La funcion es
 * propietaria del FILE que abre, por lo que debe cerrarlo tanto al completar
 * como al encontrar un error.
 */

#include <stdio.h>
#include <string.h>

#include "exercise.h"

static int save_text(const char *path, const char *text) {
  FILE *file = fopen(path, "w");
  int success;

  if (file == NULL) {
    return 0;
  }

  success = fputs(text, file) == EOF;

  if (fclose(file) == EOF) {
    success = 0;
  }

  return success;
}

int main(void) {
  const char *path = "bin/07_files_open_modes.tmp";
  char output[32] = {0};
  FILE *file;

  if (!save_text(path, "config=true\n")) {
    fprintf(stderr, "FAIL save_text\n");
    remove(path);
    return 1;
  }
  file = fopen(path, "r");
  if (file == NULL) {
    fprintf(stderr, "FAIL no se pudo reabrir el archivo\n");
    remove(path);
    return 1;
  }
  if (fgets(output, sizeof(output), file) == NULL ||
      strcmp(output, "config=true\n") != 0) {
    fprintf(stderr, "FAIL contenido guardado incorrecto\n");
    fclose(file);
    remove(path);
    return 1;
  }

  fclose(file);
  remove(path);
  return exercise_passed();
}
