/*
 * Kata 39: insertar en una tabla hash
 *
 * Implementa un registro de herramientas con separate chaining. Cada nodo
 * debe guardar una copia propia del nombre y un permiso o categoria entera.
 */

#include <stdint.h>
#include <stdlib.h>
#include <string.h>

#include "exercise.h"

#define BUCKET_COUNT 4

struct registry_node {
  char *name;
  int value;
  struct registry_node *next;
};

struct registry {
  struct registry_node *buckets[BUCKET_COUNT];
  size_t length;
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

static int registry_insert(struct registry *registry, const char *name,
                           int value) {
  (void)registry;
  (void)name;
  (void)value;

  /* TODO: copia name con malloc/memcpy, crea el nodo y enlazalo. */
  return 0;
}

int main(void) {
  struct registry registry = {0};
  size_t bucket = hash_string("read") % BUCKET_COUNT;

  if (!registry_insert(&registry, "read", 1) ||
      !registry_insert(&registry, "edit", 2) ||
      !registry_insert(&registry, "grep", 3)) {
    fprintf(stderr, "FAIL registry_insert\n");
    registry_destroy(&registry);
    return 1;
  }
  if (registry.length != 3 || hash_string("edit") % BUCKET_COUNT != bucket ||
      hash_string("grep") % BUCKET_COUNT != bucket ||
      registry.buckets[bucket] == NULL ||
      registry.buckets[bucket]->next == NULL ||
      registry.buckets[bucket]->next->next == NULL) {
    fprintf(stderr, "FAIL colisiones o length\n");
    registry_destroy(&registry);
    return 1;
  }

  registry_destroy(&registry);
  if (registry.length != 0) {
    fprintf(stderr, "FAIL registry_destroy\n");
    return 1;
  }
  return exercise_passed();
}
