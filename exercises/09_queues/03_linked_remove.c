/*
 * Kata 34: eliminar un nodo
 *
 * Elimina la primera aparicion de value. Debes reconectar la lista antes de
 * liberar el nodo y actualizar tail si eliminas el ultimo elemento.
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

static int list_append(struct list *list, int value) {
  struct node *node = malloc(sizeof(*node));

  if (node == NULL)
    return 0;
  node->value = value;
  node->next = NULL;
  if (list->tail == NULL)
    list->head = node;
  else
    list->tail->next = node;
  list->tail = node;
  list->length++;
  return 1;
}

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

static int list_remove_first(struct list *list, int value) {
  struct node *current;
  struct node *previous = NULL;

  if (list == NULL || list->length == 0) {
    return 0;
  }

  current = list->head;

  while (current != NULL) {
    if (current->value == value) {
      if (previous == NULL) {
        list->head = current->next;
      } else {
        previous->next = current->next;
      }

      if (current == list->tail) {
        list->tail = previous;
      }

      free(current);
      list->length--;

      return 1;
    }

    previous = current;
    current = current->next;
  }

  return 0;
}

int main(void) {
  struct list list = {0};

  if (!list_append(&list, 10) || !list_append(&list, 20) ||
      !list_append(&list, 30)) {
    list_destroy(&list);
    return 1;
  }
  if (!list_remove_first(&list, 20) || list.length != 2 ||
      list.head->value != 10 || list.head->next != list.tail ||
      list.tail->value != 30) {
    fprintf(stderr, "FAIL eliminar nodo intermedio\n");
    list_destroy(&list);
    return 1;
  }
  if (!list_remove_first(&list, 30) || list.tail != list.head ||
      list.tail->value != 10 || list.tail->next != NULL) {
    fprintf(stderr, "FAIL eliminar tail\n");
    list_destroy(&list);
    return 1;
  }
  if (!list_remove_first(&list, 10) || list.head != NULL || list.tail != NULL ||
      list.length != 0) {
    fprintf(stderr, "FAIL eliminar ultimo nodo\n");
    list_destroy(&list);
    return 1;
  }

  list_destroy(&list);
  return exercise_passed();
}
