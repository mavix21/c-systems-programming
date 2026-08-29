/*
 * Ejemplo: matriz respaldada por un unico bloque contiguo.
 *
 * La posicion (row, column) se traduce al indice lineal:
 * row * matrix->columns + column.
 */

#include <stdint.h>
#include <stdlib.h>

#include "exercise.h"

struct matrix {
  size_t rows;
  size_t columns;
  int *data;
};

static void matrix_destroy(struct matrix *matrix) {
  if (matrix == NULL)
    return;

  free(matrix->data);
  *matrix = (struct matrix){0};
}

/* matrix must be empty before calling matrix_init. */
static int matrix_init(struct matrix *matrix, size_t rows, size_t columns) {
  size_t element_count;
  int *data;

  if (matrix == NULL)
    return 0;

  *matrix = (struct matrix){0};
  if (rows == 0 || columns == 0)
    return 1;
  if (rows > SIZE_MAX / columns)
    return 0;

  element_count = rows * columns;
  data = calloc(element_count, sizeof(*data));
  if (data == NULL)
    return 0;

  matrix->rows = rows;
  matrix->columns = columns;
  matrix->data = data;
  return 1;
}

static int *matrix_at(struct matrix *matrix, size_t row, size_t column) {
  return &matrix->data[row * matrix->columns + column];
}

int main(void) {
  struct matrix matrix = {0};

  if (!matrix_init(&matrix, 3, 4)) {
    fprintf(stderr, "FAIL matrix_init no pudo crear la matriz\n");
    return 1;
  }

  for (size_t row = 0; row < matrix.rows; row++) {
    for (size_t column = 0; column < matrix.columns; column++) {
      if (*matrix_at(&matrix, row, column) != 0) {
        fprintf(stderr, "FAIL matrix[%zu][%zu] no vale cero\n", row, column);
        matrix_destroy(&matrix);
        return 1;
      }
    }
  }

  *matrix_at(&matrix, 1, 2) = 42;
  CHECK_INT(*matrix_at(&matrix, 1, 2), 42);
  matrix_destroy(&matrix);
  if (matrix.data != NULL || matrix.rows != 0 || matrix.columns != 0) {
    fprintf(stderr, "FAIL matrix_destroy no reinicio la matriz\n");
    return 1;
  }

  return exercise_passed();
}
