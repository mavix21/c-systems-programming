/*
 * Kata 02: punteros
 *
 * Intercambia los valores de dos enteros sin retornar ningun valor.
 * No modifiques main: usa las direcciones recibidas por swap.
 */

#include "exercise.h"

static void swap(int *left, int *right) {
  int tmp = *left;
  *left = *right;
  *right = tmp;
}

int main(void) {
  int left = 7;
  int right = 42;

  swap(&left, &right);
  CHECK_INT(left, 42);
  CHECK_INT(right, 7);

  return exercise_passed();
}
