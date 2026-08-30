/*
 * Kata 37: historial circular con ownership
 *
 * Guarda copias de los ultimos HISTORY_CAPACITY comandos. Cuando esta lleno,
 * push libera y reemplaza el mas antiguo. history_at(0) retorna el mas antiguo.
 */

#include <stdlib.h>
#include <string.h>

#include "exercise.h"

#define HISTORY_CAPACITY 3

struct command_history {
  char *items[HISTORY_CAPACITY];
  size_t head;
  size_t length;
};

static void history_destroy(struct command_history *history) {
  if (history == NULL)
    return;
  for (size_t i = 0; i < history->length; i++) {
    size_t index = (history->head + i) % HISTORY_CAPACITY;

    free(history->items[index]);
  }
  *history = (struct command_history){0};
}

static int history_push(struct command_history *history, const char *command) {
  /* TODO: copia command y publica la copia; maneja por separado lleno/no lleno.
   */
  (void)history;
  (void)command;
  return 0;
}

static const char *history_at(const struct command_history *history,
                              size_t position) {
  if (position >= history->length)
    return NULL;
  return history->items[(history->head + position) % HISTORY_CAPACITY];
}

int main(void) {
  struct command_history history = {0};
  char first[] = "help";

  if (!history_push(&history, first) ||
      !history_push(&history, "open README") ||
      !history_push(&history, "run tests") ||
      !history_push(&history, "clear")) {
    fprintf(stderr, "FAIL history_push\n");
    history_destroy(&history);
    return 1;
  }
  first[0] = 'X';
  if (history.length != 3 ||
      strcmp(history_at(&history, 0), "open README") != 0 ||
      strcmp(history_at(&history, 1), "run tests") != 0 ||
      strcmp(history_at(&history, 2), "clear") != 0 ||
      history_at(&history, 3) != NULL) {
    fprintf(stderr, "FAIL contenido del historial\n");
    history_destroy(&history);
    return 1;
  }

  history_destroy(&history);
  return exercise_passed();
}
