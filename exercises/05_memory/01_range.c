/*
 * Kata 06: memoria dinamica
 *
 * Reserva un array de length enteros y rellenalo con 0, 1, ..., length - 1.
 * Retorna NULL si no se pudo reservar. Quien llama adquiere la responsabilidad
 * de liberar el bloque.
 */

#include <stdlib.h>

#include "exercise.h"

static int *create_range(size_t length) {
  int *range = malloc(length * sizeof(int));
  if (range == NULL)
    return NULL;

  for (size_t i = 0; i < length; i++) {
    range[i] = i;
  }

  return range;
}

int main(void) {
  const size_t length = 5;
  int *values = create_range(length);

  if (values == NULL) {
    fprintf(stderr, "FAIL create_range retorno NULL\n");
    return 1;
  }

  CHECK_INT(values[0], 0);
  CHECK_INT(values[1], 1);
  CHECK_INT(values[2], 2);
  CHECK_INT(values[3], 3);
  CHECK_INT(values[4], 4);

  free(values);
  return exercise_passed();
}
