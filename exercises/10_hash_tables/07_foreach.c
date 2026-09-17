/*
 * Kata 44: recorrer un registro con callback
 *
 * Implementa registry_foreach sin asumir orden entre buckets. El callback
 * recibe un contexto opaco; retorna 0 para detener el recorrido antes de tiempo.
 */

#include <stdint.h>
#include <stdlib.h>
#include <string.h>

#include "exercise.h"

#define BUCKET_COUNT 8
#define TOOL_VISIBLE 1
#define TOOL_READ_ONLY 2

struct registry_node {
  char *name;
  int flags;
  struct registry_node *next;
};

struct registry {
  struct registry_node *buckets[BUCKET_COUNT];
  size_t length;
};

typedef int (*registry_callback)(const char *name, int flags, void *context);

struct help_metrics {
  size_t visible;
  size_t visible_read_only;
};

static uint32_t hash_string(const char *text) {
  const unsigned char *cursor = (const unsigned char *)text;
  uint32_t hash = UINT32_C(2166136261);

  while (*cursor != '\0') {
    hash ^= *cursor;
    hash *= UINT32_C(16777619);
    cursor++;
  }
  return hash;
}

static char *string_duplicate(const char *text) {
  size_t size = strlen(text) + 1;
  char *copy = malloc(size);

  if (copy != NULL)
    memcpy(copy, text, size);
  return copy;
}

static void registry_destroy(struct registry *registry) {
  if (registry == NULL)
    return;
  for (size_t i = 0; i < BUCKET_COUNT; i++) {
    struct registry_node *node = registry->buckets[i];

    while (node != NULL) {
      struct registry_node *next = node->next;

      free(node->name);
      free(node);
      node = next;
    }
  }
  *registry = (struct registry){0};
}

static int registry_put(struct registry *registry, const char *name, int flags) {
  size_t bucket;
  struct registry_node *node;

  if (registry == NULL || name == NULL)
    return 0;
  bucket = hash_string(name) % BUCKET_COUNT;
  for (node = registry->buckets[bucket]; node != NULL; node = node->next) {
    if (strcmp(node->name, name) == 0) {
      node->flags = flags;
      return 1;
    }
  }
  node = malloc(sizeof(*node));
  if (node == NULL)
    return 0;
  node->name = string_duplicate(name);
  if (node->name == NULL) {
    free(node);
    return 0;
  }
  node->flags = flags;
  node->next = registry->buckets[bucket];
  registry->buckets[bucket] = node;
  registry->length++;
  return 1;
}

static int registry_foreach(const struct registry *registry,
                            registry_callback callback, void *context) {
  (void)registry;
  (void)callback;
  (void)context;

  /* TODO: visita cada nodo; retorna 0 si callback pide detenerse. */
  return 0;
}

static int collect_help_metrics(const char *name, int flags, void *context) {
  struct help_metrics *metrics = context;

  (void)name;
  if ((flags & TOOL_VISIBLE) != 0) {
    metrics->visible++;
    if ((flags & TOOL_READ_ONLY) != 0)
      metrics->visible_read_only++;
  }
  return 1;
}

static int stop_after_one(const char *name, int flags, void *context) {
  size_t *calls = context;

  (void)name;
  (void)flags;
  (*calls)++;
  return 0;
}

int main(void) {
  struct registry registry = {0};
  struct help_metrics metrics = {0};
  size_t calls = 0;

  if (!registry_put(&registry, "read", TOOL_VISIBLE | TOOL_READ_ONLY) ||
      !registry_put(&registry, "grep", TOOL_VISIBLE | TOOL_READ_ONLY) ||
      !registry_put(&registry, "edit", TOOL_VISIBLE) ||
      !registry_put(&registry, "internal_trace", TOOL_READ_ONLY)) {
    fprintf(stderr, "FAIL preparacion del registro\n");
    registry_destroy(&registry);
    return 1;
  }
  if (!registry_foreach(&registry, collect_help_metrics, &metrics) ||
      metrics.visible != 3 || metrics.visible_read_only != 2) {
    fprintf(stderr, "FAIL metricas de ayuda\n");
    registry_destroy(&registry);
    return 1;
  }
  if (registry_foreach(&registry, stop_after_one, &calls) || calls != 1 ||
      registry_foreach(NULL, collect_help_metrics, &metrics) ||
      registry_foreach(&registry, NULL, &metrics)) {
    fprintf(stderr, "FAIL parada temprana o NULL safety\n");
    registry_destroy(&registry);
    return 1;
  }

  registry_destroy(&registry);
  return exercise_passed();
}
