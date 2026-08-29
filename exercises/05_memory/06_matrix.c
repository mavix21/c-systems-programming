/*
 * Kata 11: multiples reservas y cleanup
 *
 * Inicializa una matriz de rows por columns con todos sus enteros a cero. Cada
 * fila debe tener su propia reserva. Si una reserva falla, libera todo lo que
 * ya se hubiera obtenido. matrix_destroy debe aceptar una matriz parcial o
 * vacia y dejarla en un estado reutilizable.
 */

#include <stdlib.h>

#include "exercise.h"

struct matrix {
  size_t rows;
  size_t columns;
  int **data;
};

static void matrix_destroy(struct matrix *matrix) {
  if (matrix == NULL)
    return;

  if (matrix->data == NULL) {
    *matrix = (struct matrix){0};

    return;
  }

  size_t rows = matrix->rows;
  int **cursor = matrix->data;

  for (size_t i = 0; i < rows; i++, cursor++) {
    free(*cursor);
  }

  free(matrix->data);
  *matrix = (struct matrix){0};
}

static int matrix_init(struct matrix *matrix, size_t rows, size_t columns) {
  matrix->rows = rows;
  matrix->columns = columns;

  int **arr_ptr = calloc(rows, sizeof(*matrix->data));
  if (arr_ptr == NULL)
    return 0;

  matrix->data = arr_ptr;

  for (size_t i = 0; i < rows; i++) {
    arr_ptr[i] = calloc(columns, sizeof(**matrix->data));
    if (arr_ptr[i] == NULL) {
      matrix_destroy(matrix);
      return 0;
    }
  }

  return 1;
}

int main(void) {
  struct matrix matrix = {0};

  if (!matrix_init(&matrix, 3, 4)) {
    fprintf(stderr, "FAIL matrix_init no pudo crear la matriz\n");
    matrix_destroy(&matrix);
    return 1;
  }
  if (matrix.rows != 3 || matrix.columns != 4 || matrix.data == NULL) {
    fprintf(stderr, "FAIL dimensiones o data incorrectos\n");
    matrix_destroy(&matrix);
    return 1;
  }
  for (size_t row = 0; row < matrix.rows; row++) {
    for (size_t column = 0; column < matrix.columns; column++) {
      if (matrix.data[row][column] != 0) {
        fprintf(stderr, "FAIL matrix[%zu][%zu] no vale cero\n", row, column);
        matrix_destroy(&matrix);
        return 1;
      }
    }
  }

  matrix.data[1][2] = 42;
  CHECK_INT(matrix.data[1][2], 42);
  matrix_destroy(&matrix);
  if (matrix.data != NULL || matrix.rows != 0 || matrix.columns != 0) {
    fprintf(stderr, "FAIL matrix_destroy no reinicio la matriz\n");
    return 1;
  }

  return exercise_passed();
}
