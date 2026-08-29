/*
 * Kata 04: strings
 *
 * Calcula la longitud de una string sin usar strlen. Recuerda que una string
 * de C termina en el byte nulo '\0', que no cuenta como parte de su longitud.
 */

#include "exercise.h"

static size_t string_length(const char *text) {
  size_t len = 0;

  while (*text != '\0') {
    len++;
    text++;
  }

  return len;
}

int main(void) {
  CHECK_SIZE(string_length(""), 0);
  CHECK_SIZE(string_length("C"), 1);
  CHECK_SIZE(string_length("punteros"), 8);

  return exercise_passed();
}
