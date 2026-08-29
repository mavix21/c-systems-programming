/*
 * Kata 09: realloc seguro
 *
 * Amplia un array de old_length a new_length. Conserva sus valores e
 * inicializa a cero las posiciones nuevas. Retorna 1 al completar la operacion
 * y 0 si falla; ante un fallo, el puntero original debe seguir siendo valido.
 * Para esta kata puedes asumir que new_length es mayor que old_length.
 */

#include <stdlib.h>

#include "exercise.h"

static int grow_array(int **values, size_t old_length, size_t new_length) {
  /* TODO: usa realloc sin perder el bloque original si la reserva falla. */
  int *resized = realloc(*values, new_length * sizeof(**values));
  if (resized == NULL) {
    return 0;
  }

  for (size_t i = old_length; i < new_length; i++) {
    resized[i] = 0;
  }

  *values = resized;

  return 1;
}

int main(void) {
  const size_t old_length = 3;
  const size_t new_length = 6;
  int *values = malloc(old_length * sizeof(*values));

  if (values == NULL) {
    fprintf(stderr, "FAIL malloc inicial\n");
    return 1;
  }
  values[0] = 10;
  values[1] = 20;
  values[2] = 30;

  if (!grow_array(&values, old_length, new_length)) {
    fprintf(stderr, "FAIL grow_array no pudo ampliar el array\n");
    free(values);
    return 1;
  }
  if (values[0] != 10 || values[1] != 20 || values[2] != 30) {
    fprintf(stderr, "FAIL grow_array no conservo los valores\n");
    free(values);
    return 1;
  }
  for (size_t i = old_length; i < new_length; i++) {
    if (values[i] != 0) {
      fprintf(stderr, "FAIL values[%zu] no esta inicializado a cero\n", i);
      free(values);
      return 1;
    }
  }

  free(values);
  return exercise_passed();
}
