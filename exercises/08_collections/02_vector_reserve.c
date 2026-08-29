/*
 * Kata 25: reservar elementos
 *
 * Asegura espacio para additional elementos a partir de length. Conserva el
 * contenido y length. Usa crecimiento geometrico y evita overflow.
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
  size_t new_required_capacity = 0;

  if (vector == NULL) {
    return 0;
  }

  if (vector->length == SIZE_MAX ||
      additional > SIZE_MAX - vector->length - 1) {
    return 0;
  }

  new_required_capacity = vector->length + additional + 1;

  if (vector->length + additional <= vector->capacity) {
    return 0;
  }

  while (1) {
    if (new_required_capacity > SIZE_MAX / 2) {
    }

    new_required_capacity *= 2;
  }

  return 0;
}

int main(void) {
  struct int_vector vector = {0};

  vector.items = malloc(2 * sizeof(*vector.items));
  if (vector.items == NULL)
    return 1;
  vector.items[0] = 10;
  vector.items[1] = 20;
  vector.length = 2;
  vector.capacity = 2;

  if (!vector_reserve(&vector, 8)) {
    fprintf(stderr, "FAIL vector_reserve\n");
    vector_destroy(&vector);
    return 1;
  }
  if (vector.capacity < 10 || vector.length != 2 || vector.items[0] != 10 ||
      vector.items[1] != 20) {
    fprintf(stderr, "FAIL estado posterior a reserve incorrecto\n");
    vector_destroy(&vector);
    return 1;
  }

  vector_destroy(&vector);
  return exercise_passed();
}
