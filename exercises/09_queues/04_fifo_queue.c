/*
 * Kata 35: cola FIFO
 *
 * enqueue inserta al final y dequeue retira del principio. Ambas operaciones
 * deben ser O(1). Al retirar el ultimo nodo, head y tail vuelven a NULL.
 */

#include <stdlib.h>

#include "exercise.h"

struct queue_node {
  int value;
  struct queue_node *next;
};

struct queue {
  struct queue_node *head;
  struct queue_node *tail;
  size_t length;
};

static int queue_enqueue(struct queue *queue, int value) {
  /* TODO: reserva y enlaza un nodo al tail. */
  (void)queue;
  (void)value;
  return 0;
}

static int queue_dequeue(struct queue *queue, int *out_value) {
  /* TODO: retira head, publica su valor y libera el nodo. */
  (void)queue;
  (void)out_value;
  return 0;
}

static void queue_destroy(struct queue *queue) {
  int ignored;

  if (queue == NULL)
    return;
  while (queue_dequeue(queue, &ignored)) {
  }
}

int main(void) {
  struct queue queue = {0};
  int value;

  if (!queue_enqueue(&queue, 10) || !queue_enqueue(&queue, 20) ||
      !queue_enqueue(&queue, 30)) {
    fprintf(stderr, "FAIL queue_enqueue\n");
    queue_destroy(&queue);
    return 1;
  }
  for (int expected = 10; expected <= 30; expected += 10) {
    if (!queue_dequeue(&queue, &value) || value != expected) {
      fprintf(stderr, "FAIL orden FIFO\n");
      queue_destroy(&queue);
      return 1;
    }
  }
  if (queue_dequeue(&queue, &value) || queue.head != NULL ||
      queue.tail != NULL || queue.length != 0) {
    fprintf(stderr, "FAIL estado de cola vacia\n");
    queue_destroy(&queue);
    return 1;
  }

  queue_destroy(&queue);
  return exercise_passed();
}
