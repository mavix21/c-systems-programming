/*
 * Kata 32: lista enlazada y push_front
 *
 * Reserva un nodo y enlazalo al principio. list_destroy debe recorrer todos los
 * nodos, liberarlos y dejar la lista vacia.
 */

#include <stdlib.h>

#include "exercise.h"

struct node {
  int value;
  struct node *next;
};

struct list {
  struct node *head;
  size_t length;
};

static int list_push_front(struct list *list, int value) {
  /* TODO: reserva un nodo y conviertelo en el nuevo head. */
  (void)list;
  (void)value;
  return 0;
}

static void list_destroy(struct list *list) {
  /* TODO: guarda next antes de liberar cada nodo y reinicia la lista. */
  (void)list;
}

int main(void) {
  struct list list = {0};
  struct node *cursor;
  const int expected[] = {30, 20, 10};

  if (!list_push_front(&list, 10) || !list_push_front(&list, 20) ||
      !list_push_front(&list, 30)) {
    fprintf(stderr, "FAIL list_push_front\n");
    list_destroy(&list);
    return 1;
  }
  if (list.length != 3) {
    fprintf(stderr, "FAIL length esperado 3\n");
    list_destroy(&list);
    return 1;
  }
  cursor = list.head;
  for (size_t i = 0; i < 3; i++) {
    if (cursor == NULL || cursor->value != expected[i]) {
      fprintf(stderr, "FAIL nodo %zu\n", i);
      list_destroy(&list);
      return 1;
    }
    cursor = cursor->next;
  }

  list_destroy(&list);
  if (list.head != NULL || list.length != 0) {
    fprintf(stderr, "FAIL list_destroy\n");
    return 1;
  }
  return exercise_passed();
}
