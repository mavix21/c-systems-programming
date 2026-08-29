/*
 * Kata 08: copia y ownership
 *
 * Crea en el heap una copia independiente de text sin usar strdup. Incluye el
 * byte nulo final. Quien llama pasa a ser responsable de liberar la copia.
 */

#include <stdlib.h>
#include <string.h>

#include "exercise.h"

static char *duplicate_string(const char *text) {
  /* TODO: reserva el espacio exacto y copia todos los bytes necesarios. */
  char *duplicate = malloc(strlen(text) + 1 * sizeof(char));
  if (duplicate == NULL)
    return NULL;

  char *anchor = duplicate;
  while (*text != '\0') {
    *duplicate = *text;
    duplicate++;
    text++;
  }
  *duplicate = '\0';
  duplicate = anchor;

  return duplicate;
}

int main(void) {
  const char original[] = "memoria";
  char *copy = duplicate_string(original);

  if (copy == NULL) {
    fprintf(stderr, "FAIL duplicate_string retorno NULL\n");
    return 1;
  }
  if (copy == original || strcmp(copy, original) != 0) {
    fprintf(stderr, "FAIL la copia no es independiente o no coincide\n");
    free(copy);
    return 1;
  }

  copy[0] = 'M';
  if (original[0] != 'm') {
    fprintf(stderr, "FAIL modificar la copia altero el original\n");
    free(copy);
    return 1;
  }

  free(copy);
  return exercise_passed();
}
