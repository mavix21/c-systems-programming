/*
 * Kata 33: append en O(1)
 *
 * Mantener tail permite insertar al final sin recorrer la lista. Actualiza
 * correctamente head y tail al insertar el primer nodo.
 */

#include <stdlib.h>

#include "exercise.h"

struct node {
  int value;
  struct node *next;
};

struct list {
  struct node *head;
  struct node *tail;
  size_t length;
};

static void list_destroy(struct list *list) {
  struct node *cursor;

  if (list == NULL)
    return;
  cursor = list->head;
  while (cursor != NULL) {
    struct node *next = cursor->next;

    free(cursor);
    cursor = next;
  }
  *list = (struct list){0};
}

static int list_append(struct list *list, int value) {
  struct node *new_node;

  if (list == NULL) {
    return 0;
  }

  new_node = malloc(sizeof(struct node));
  if (new_node == NULL) {
    return 0;
  }

  new_node->value = value;
  new_node->next = NULL;

  if (list->head == NULL) {
    list->head = new_node;
    list->tail = new_node;
  } else {
    list->tail->next = new_node;
    list->tail = new_node;
  }

  list->length++;

  return 1;
}

int main(void) {
  struct list list = {0};
  struct node *cursor;
  const int expected[] = {10, 20, 30};

  if (!list_append(&list, 10) || !list_append(&list, 20) ||
      !list_append(&list, 30)) {
    fprintf(stderr, "FAIL list_append\n");
    list_destroy(&list);
    return 1;
  }
  cursor = list.head;
  for (size_t i = 0; i < 3; i++) {
    if (cursor == NULL || cursor->value != expected[i]) {
      fprintf(stderr, "FAIL orden en indice %zu\n", i);
      list_destroy(&list);
      return 1;
    }
    cursor = cursor->next;
  }
  if (list.length != 3 || list.tail == NULL || list.tail->value != 30 ||
      list.tail->next != NULL) {
    fprintf(stderr, "FAIL metadata de la lista\n");
    list_destroy(&list);
    return 1;
  }

  list_destroy(&list);
  return exercise_passed();
}
