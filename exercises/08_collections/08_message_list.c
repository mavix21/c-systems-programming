/*
 * Kata 31: historial de mensajes
 *
 * Anade mensajes con role y una copia propia de content. Si cualquier reserva
 * falla, la lista debe permanecer valida y length no debe aumentar.
 */

#include <stdint.h>
#include <stdlib.h>
#include <string.h>

#include "exercise.h"

enum message_role { MESSAGE_SYSTEM, MESSAGE_USER, MESSAGE_ASSISTANT };

struct message {
  enum message_role role;
  char *content;
};

struct message_list {
  struct message *items;
  size_t length;
  size_t capacity;
};

static void message_list_destroy(struct message_list *list) {
  if (list == NULL)
    return;
  for (size_t i = 0; i < list->length; i++)
    free(list->items[i].content);
  free(list->items);
  *list = (struct message_list){0};
}

static int message_list_reserve(struct message_list *list, size_t additional) {
  size_t required;
  size_t capacity;
  struct message *resized;

  if (additional > SIZE_MAX - list->length)
    return 0;
  required = list->length + additional;
  if (required <= list->capacity)
    return 1;
  capacity = list->capacity == 0 ? 4 : list->capacity;
  while (capacity < required) {
    if (capacity > SIZE_MAX / 2) {
      capacity = required;
      break;
    }
    capacity *= 2;
  }
  if (capacity > SIZE_MAX / sizeof(*resized))
    return 0;
  resized = realloc(list->items, capacity * sizeof(*resized));
  if (resized == NULL)
    return 0;
  list->items = resized;
  list->capacity = capacity;
  return 1;
}

static int message_list_push(struct message_list *list, enum message_role role,
                             const char *content) {
  struct message new_message = {.role = role};
  char *new_content;

  if (list == NULL || content == NULL) {
    return 0;
  }

  /* TODO: reserva un slot, copia content y publica el mensaje al final. */
  if (!message_list_reserve(list, 1))
    return 0;

  new_content = malloc((strlen(content) + 1) * sizeof(*new_content));
  if (new_content == NULL) {
    return 0;
  }

  strcpy(new_content, content);
  new_message.content = new_content;

  list->items[list->length] = new_message;
  list->length++;

  return 1;
}

int main(void) {
  struct message_list messages = {0};
  char prompt[] = "Explica realloc";

  if (!message_list_push(&messages, MESSAGE_SYSTEM, "Responde breve") ||
      !message_list_push(&messages, MESSAGE_USER, prompt)) {
    fprintf(stderr, "FAIL message_list_push\n");
    message_list_destroy(&messages);
    return 1;
  }
  prompt[0] = 'X';
  if (messages.length != 2 || messages.items[0].role != MESSAGE_SYSTEM ||
      strcmp(messages.items[0].content, "Responde breve") != 0 ||
      messages.items[1].role != MESSAGE_USER ||
      strcmp(messages.items[1].content, "Explica realloc") != 0) {
    fprintf(stderr, "FAIL historial de mensajes\n");
    message_list_destroy(&messages);
    return 1;
  }

  message_list_destroy(&messages);
  return exercise_passed();
}
