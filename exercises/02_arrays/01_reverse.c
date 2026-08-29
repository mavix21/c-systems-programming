/*
 * Kata 03: arrays y aritmetica de indices
 *
 * Invierte el array in-place. No crees un segundo array y no asumas una
 * longitud fija.
 */

#include "exercise.h"

static void reverse(int *values, size_t length) {
  int start = 0;
  int end = length - 1;
  int temp;

  while (start < end) {
    temp = values[start];
    values[start] = values[end];
    values[end] = temp;

    start++;
    end--;
  }
}

int main(void) {
  int values[] = {1, 2, 3, 4, 5};

  reverse(values, sizeof(values) / sizeof(values[0]));
  CHECK_INT(values[0], 5);
  CHECK_INT(values[1], 4);
  CHECK_INT(values[2], 3);
  CHECK_INT(values[3], 2);
  CHECK_INT(values[4], 1);

  return exercise_passed();
}
