/*
 * Kata 24: ciclo de vida de un vector
 *
 * Inicializa un vector vacio con espacio para initial_capacity enteros. Sus
 * elementos deben comenzar a cero. vector_destroy libera y reinicia el struct.
 */

#include <stdlib.h>

#include "exercise.h"

struct int_vector {
  int *items;
  size_t length;
  size_t capacity;
};

static int vector_init(struct int_vector *vector, size_t initial_capacity) {
  int *items;

  if (vector == NULL)
    return 0;

  items = calloc(initial_capacity, sizeof(*vector->items));
  if (items == NULL) {
    return 0;
  }

  vector->items = items;
  vector->capacity = initial_capacity;
  vector->length = 0;

  return 1;
}

static void vector_destroy(struct int_vector *vector) {
  if (vector == NULL)
    return;

  free(vector->items);
  *vector = (struct int_vector){0};
}

int main(void) {
  struct int_vector vector = {0};

  if (!vector_init(&vector, 4)) {
    fprintf(stderr, "FAIL vector_init\n");
    return 1;
  }
  if (vector.items == NULL || vector.length != 0 || vector.capacity != 4) {
    fprintf(stderr, "FAIL estado inicial incorrecto\n");
    vector_destroy(&vector);
    return 1;
  }
  for (size_t i = 0; i < vector.capacity; i++) {
    if (vector.items[i] != 0) {
      fprintf(stderr, "FAIL items[%zu] no vale cero\n", i);
      vector_destroy(&vector);
      return 1;
    }
  }

  vector_destroy(&vector);
  if (vector.items != NULL || vector.length != 0 || vector.capacity != 0) {
    fprintf(stderr, "FAIL vector_destroy no reinicio el vector\n");
    return 1;
  }
  return exercise_passed();
}
