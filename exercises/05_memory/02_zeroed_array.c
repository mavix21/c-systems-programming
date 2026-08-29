/*
 * Kata 07: memoria inicializada
 *
 * Reserva length enteros inicializados a cero. Retorna NULL si la reserva
 * falla. Puedes usar calloc o combinar malloc con inicializacion manual.
 */

#include <stdlib.h>

#include "exercise.h"

static int *allocate_zeroes(size_t length) {
  /* TODO: reserva e inicializa length enteros. */
  int *values = calloc(length, sizeof(int));

  if (values == NULL)
    return NULL;

  return values;
}

int main(void) {
  const size_t length = 8;
  int *values = allocate_zeroes(length);

  if (values == NULL) {
    fprintf(stderr, "FAIL allocate_zeroes retorno NULL\n");
    return 1;
  }

  for (size_t i = 0; i < length; i++) {
    if (values[i] != 0) {
      fprintf(stderr, "FAIL values[%zu] no esta inicializado a cero\n", i);
      free(values);
      return 1;
    }
  }

  free(values);
  return exercise_passed();
}
