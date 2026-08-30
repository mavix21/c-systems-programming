/*
 * Kata 36: ring buffer de capacidad fija
 *
 * Usa head y length para interpretar el array de forma circular. push retorna
 * 0 si esta lleno; pop retorna 0 si esta vacio. Ninguna operacion mueve datos.
 */

#include "exercise.h"

#define RING_CAPACITY 4

struct ring_buffer {
  int items[RING_CAPACITY];
  size_t head;
  size_t length;
};

static int ring_push(struct ring_buffer *ring, int value) {
  /* TODO: calcula tail como (head + length) modulo capacity. */
  (void)ring;
  (void)value;
  return 0;
}

static int ring_pop(struct ring_buffer *ring, int *out_value) {
  /* TODO: lee head, avanzalo circularmente y reduce length. */
  (void)ring;
  (void)out_value;
  return 0;
}

int main(void) {
  struct ring_buffer ring = {0};
  int value;

  for (int i = 1; i <= 4; i++) {
    if (!ring_push(&ring, i)) {
      fprintf(stderr, "FAIL push inicial\n");
      return 1;
    }
  }
  if (ring_push(&ring, 5) || !ring_pop(&ring, &value) || value != 1 ||
      !ring_pop(&ring, &value) || value != 2) {
    fprintf(stderr, "FAIL lleno o pop\n");
    return 1;
  }
  if (!ring_push(&ring, 5) || !ring_push(&ring, 6)) {
    fprintf(stderr, "FAIL wrap-around\n");
    return 1;
  }
  for (int expected = 3; expected <= 6; expected++) {
    if (!ring_pop(&ring, &value) || value != expected) {
      fprintf(stderr, "FAIL orden circular\n");
      return 1;
    }
  }
  if (ring_pop(&ring, &value)) {
    fprintf(stderr, "FAIL pop vacio\n");
    return 1;
  }
  return exercise_passed();
}
