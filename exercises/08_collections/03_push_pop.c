/*
 * Kata 26: push y pop
 *
 * push anade al final, creciendo cuando sea necesario. pop elimina el ultimo
 * elemento y lo escribe en out_value. pop retorna 0 si el vector esta vacio.
 */

#include <stdint.h>
#include <stdlib.h>

#include "exercise.h"

struct int_vector {
  int *items;
  size_t length;
  size_t capacity;
};

static void vector_destroy(struct int_vector *vector) {
  if (vector == NULL)
    return;
  free(vector->items);
  *vector = (struct int_vector){0};
}

static int vector_reserve(struct int_vector *vector, size_t additional) {
  size_t required;
  size_t new_capacity;
  int *resized;

  if (additional > SIZE_MAX - vector->length)
    return 0;

  required = vector->length + additional;
  if (required <= vector->capacity)
    return 1;

  new_capacity = vector->capacity == 0 ? 4 : vector->capacity;
  while (new_capacity < required) {
    if (new_capacity > SIZE_MAX / 2) {
      new_capacity = required;
      break;
    }
    new_capacity *= 2;
  }

  if (new_capacity > SIZE_MAX / sizeof(*resized))
    return 0;

  resized = realloc(vector->items, new_capacity * sizeof(*resized));
  if (resized == NULL)
    return 0;

  vector->items = resized;
  vector->capacity = new_capacity;

  return 1;
}

static int vector_push(struct int_vector *vector, int value) {
  /* TODO: reserva un elemento, escribe value y aumenta length. */
  if (!vector_reserve(vector, 1))
    return 0;

  vector->items[vector->length] = value;
  vector->length++;

  return 0;
}

static int vector_pop(struct int_vector *vector, int *out_value) {
  /* TODO: maneja el vector vacio y retira el ultimo elemento. */
  (void)vector;
  (void)out_value;
  return 0;
}

int main(void) {
  struct int_vector vector = {0};
  int value = 0;

  if (!vector_push(&vector, 10) || !vector_push(&vector, 20) ||
      !vector_push(&vector, 30)) {
    fprintf(stderr, "FAIL vector_push\n");
    vector_destroy(&vector);
    return 1;
  }
  if (!vector_pop(&vector, &value) || value != 30 || vector.length != 2) {
    fprintf(stderr, "FAIL vector_pop\n");
    vector_destroy(&vector);
    return 1;
  }
  if (vector.items[0] != 10 || vector.items[1] != 20) {
    fprintf(stderr, "FAIL contenido del vector\n");
    vector_destroy(&vector);
    return 1;
  }

  vector_destroy(&vector);
  return exercise_passed();
}
