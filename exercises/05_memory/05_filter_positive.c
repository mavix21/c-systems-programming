/*
 * Kata 10: reserva con tamano calculado
 *
 * Copia solo los numeros positivos en un array nuevo de tamano exacto. Guarda
 * la longitud resultante en out_length. Si no hay positivos, asigna cero a
 * out_length y retorna NULL. Retorna NULL tambien si la reserva falla.
 */

#include <stdlib.h>

#include "exercise.h"

static int *copy_positive(const int *values, size_t length,
                          size_t *out_length) {
  /* TODO: cuenta primero, reserva el tamano exacto y despues copia. */
  size_t positives = 0;
  for (size_t i = 0; i < length; i++) {
    if (values[i] > 0)
      positives++;
  }
  if (positives == 0) {
    *out_length = 0;
    return NULL;
  }

  int *out_values = malloc(positives * sizeof(*values));
  if (out_values == NULL)
    return NULL;

  for (size_t i = 0, j = 0; i < length && j < positives; i++) {
    if (values[i] > 0) {
      out_values[j++] = values[i];
    }
  }
  *out_length = positives;

  return out_values;
}

int main(void) {
  const int source[] = {-4, 8, 0, 15, -16, 23};
  const int expected[] = {8, 15, 23};
  size_t result_length = 0;
  int *result =
      copy_positive(source, sizeof(source) / sizeof(source[0]), &result_length);

  if (result == NULL) {
    fprintf(stderr, "FAIL copy_positive retorno NULL\n");
    return 1;
  }
  if (result_length != sizeof(expected) / sizeof(expected[0])) {
    fprintf(stderr, "FAIL longitud esperada 3, obtenida %zu\n", result_length);
    free(result);
    return 1;
  }
  for (size_t i = 0; i < result_length; i++) {
    if (result[i] != expected[i]) {
      fprintf(stderr, "FAIL result[%zu] tiene un valor incorrecto\n", i);
      free(result);
      return 1;
    }
  }

  free(result);
  return exercise_passed();
}
