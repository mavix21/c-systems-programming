/*
 * Kata 05: structs y paso por puntero
 *
 * Actualiza la edad de una persona. Si su cumpleanos todavia no ocurrio este
 * ano, conserva la edad actual; si ya ocurrio, incrementala en uno.
 */

#include "exercise.h"

struct person {
  const char *name;
  int age;
};

static void apply_birthday(struct person *person, int already_happened) {
  /* TODO: actualiza person solo cuando already_happened sea distinto de 0. */
  if (already_happened == 0)
    return;

  person->age++;
}

int main(void) {
  struct person ada = {"Ada", 35};
  struct person ken = {"Ken", 82};

  apply_birthday(&ada, 1);
  apply_birthday(&ken, 0);
  CHECK_INT(ada.age, 36);
  CHECK_INT(ken.age, 82);

  return exercise_passed();
}
