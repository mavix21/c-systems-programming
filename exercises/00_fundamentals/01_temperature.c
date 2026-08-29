/*
 * Kata 01: tipos y funciones
 *
 * Convierte grados Celsius a Fahrenheit usando F = C * 9 / 5 + 32.
 * Piensa en el orden de las operaciones y en la aritmetica de enteros.
 */

#include "exercise.h"

static int celsius_to_fahrenheit(int celsius) {
  /* TODO: implementa la conversion. */
  return (celsius * 9) / 5 + 32;
}

int main(void) {
  CHECK_INT(celsius_to_fahrenheit(0), 32);
  CHECK_INT(celsius_to_fahrenheit(100), 212);
  CHECK_INT(celsius_to_fahrenheit(-40), -40);

  return exercise_passed();
}
