/*
 * Kata 43: crecer y redistribuir buckets
 *
 * El registro crece al superar 75% de carga. Implementa registry_rehash para
 * mover los nodos existentes sin copiar ni liberar sus nombres.
 */

#include <stdint.h>
#include <stdlib.h>
#include <string.h>

#include "exercise.h"

#define INITIAL_CAPACITY 4

struct registry_node {
  char *name;
  int value;
  struct registry_node *next;
};

struct registry {
  struct registry_node **buckets;
  size_t length;
  size_t capacity;
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

static int registry_init(struct registry *registry) {
  if (registry == NULL)
    return 0;
  *registry = (struct registry){0};
  registry->buckets = calloc(INITIAL_CAPACITY, sizeof(*registry->buckets));
  if (registry->buckets == NULL)
    return 0;
  registry->capacity = INITIAL_CAPACITY;
  return 1;
}

static void registry_destroy(struct registry *registry) {
  if (registry == NULL)
    return;
  for (size_t i = 0; i < registry->capacity; i++) {
    struct registry_node *node = registry->buckets[i];

    while (node != NULL) {
      struct registry_node *next = node->next;

      free(node->name);
      free(node);
      node = next;
    }
  }
  free(registry->buckets);
  *registry = (struct registry){0};
}

static int registry_get(const struct registry *registry, const char *name,
                        int *out_value) {
  struct registry_node *node;

  if (registry == NULL || registry->buckets == NULL || name == NULL ||
      out_value == NULL)
    return 0;
  node = registry->buckets[hash_string(name) % registry->capacity];
  while (node != NULL) {
    if (strcmp(node->name, name) == 0) {
      *out_value = node->value;
      return 1;
    }
    node = node->next;
  }
  return 0;
}

static int registry_rehash(struct registry *registry, size_t new_capacity) {
  (void)registry;
  (void)new_capacity;

  /* TODO: reserva buckets nuevos y redistribuye los mismos nodos. */
  return 0;
}

static int registry_put(struct registry *registry, const char *name,
                        int value) {
  size_t bucket;
  struct registry_node *node;

  if (registry == NULL || registry->buckets == NULL || name == NULL)
    return 0;
  bucket = hash_string(name) % registry->capacity;
  for (node = registry->buckets[bucket]; node != NULL; node = node->next) {
    if (strcmp(node->name, name) == 0) {
      node->value = value;
      return 1;
    }
  }

  if (registry->length + 1 > registry->capacity - registry->capacity / 4) {
    if (registry->capacity > SIZE_MAX / 2 ||
        !registry_rehash(registry, registry->capacity * 2))
      return 0;
    bucket = hash_string(name) % registry->capacity;
  }

  node = malloc(sizeof(*node));
  if (node == NULL)
    return 0;
  node->name = string_duplicate(name);
  if (node->name == NULL) {
    free(node);
    return 0;
  }
  node->value = value;
  node->next = registry->buckets[bucket];
  registry->buckets[bucket] = node;
  registry->length++;
  return 1;
}

int main(void) {
  struct registry registry = {0};
  int value;

  if (!registry_init(&registry)) {
    fprintf(stderr, "FAIL registry_init\n");
    return 1;
  }
  if (!registry_put(&registry, "read", 1) ||
      !registry_put(&registry, "edit", 2) ||
      !registry_put(&registry, "grep", 3) ||
      !registry_put(&registry, "shell", 4)) {
    fprintf(stderr, "FAIL insercion o crecimiento\n");
    registry_destroy(&registry);
    return 1;
  }
  if (registry.capacity != 8 || registry.length != 4 ||
      !registry_get(&registry, "read", &value) || value != 1 ||
      !registry_get(&registry, "edit", &value) || value != 2 ||
      !registry_get(&registry, "grep", &value) || value != 3 ||
      !registry_get(&registry, "shell", &value) || value != 4) {
    fprintf(stderr, "FAIL capacidad, length o preservacion de lookups\n");
    registry_destroy(&registry);
    return 1;
  }

  registry_destroy(&registry);
  return exercise_passed();
}
