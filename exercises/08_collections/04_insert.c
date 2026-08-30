/*
 * Kata 27: insertar y desplazar
 *
 * Inserta value en index. index puede estar entre 0 y length inclusive. Mueve
 * los elementos existentes con memmove, que admite regiones solapadas.
 */

#include <stdint.h>
#include <stdlib.h>
#include <string.h>

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
  size_t capacity;
  int *resized;

  if (additional > SIZE_MAX - vector->length)
    return 0;
  required = vector->length + additional;
  if (required <= vector->capacity)
    return 1;
  capacity = vector->capacity == 0 ? 4 : vector->capacity;
  while (capacity < required) {
    if (capacity > SIZE_MAX / 2) {
      capacity = required;
      break;
    }
    capacity *= 2;
  }
  if (capacity > SIZE_MAX / sizeof(*resized))
    return 0;
  resized = realloc(vector->items, capacity * sizeof(*resized));
  if (resized == NULL)
    return 0;
  vector->items = resized;
  vector->capacity = capacity;
  return 1;
}

static int vector_push(struct int_vector *vector, int value) {
  if (!vector_reserve(vector, 1))
    return 0;
  vector->items[vector->length++] = value;
  return 1;
}

static int vector_insert(struct int_vector *vector, size_t index, int value) {
  if (vector == NULL || index > vector->length) {
    return 0;
  }

  if (!vector_reserve(vector, 1)) {
    return 0;
  }

  memmove(&vector->items[index + 1], &vector->items[index],
          (vector->length - index) * sizeof(*vector->items));

  vector->items[index] = value;
  vector->length++;

  return 1;
}

int main(void) {
  struct int_vector vector = {0};
  const int expected[] = {10, 20, 30, 40};

  if (!vector_push(&vector, 10) || !vector_push(&vector, 30) ||
      !vector_push(&vector, 40) || !vector_insert(&vector, 1, 20)) {
    fprintf(stderr, "FAIL preparacion o insert\n");
    vector_destroy(&vector);
    return 1;
  }
  if (vector.length != 4) {
    fprintf(stderr, "FAIL length esperado 4\n");
    vector_destroy(&vector);
    return 1;
  }
  for (size_t i = 0; i < vector.length; i++) {
    if (vector.items[i] != expected[i]) {
      fprintf(stderr, "FAIL items[%zu]\n", i);
      vector_destroy(&vector);
      return 1;
    }
  }

  vector_destroy(&vector);
  return exercise_passed();
}
