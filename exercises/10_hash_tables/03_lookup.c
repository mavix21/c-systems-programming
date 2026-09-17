/*
 * Kata 40: buscar en una tabla hash
 *
 * Completa registry_get. Calcula un solo bucket, recorre su cadena y entrega
 * el valor mediante out_value sin modificarlo cuando el nombre no existe.
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

static int registry_put(struct registry *registry, const char *name,
                        int value) {
  size_t bucket;
  struct registry_node *node;

  if (registry == NULL || name == NULL)
    return 0;
  bucket = hash_string(name) % BUCKET_COUNT;
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

static int registry_get(const struct registry *registry, const char *name,
                        int *out_value) {
  (void)registry;
  (void)name;
  (void)out_value;

  /* TODO: recorre solo hash_string(name) % BUCKET_COUNT. */
  return 0;
}

int main(void) {
  struct registry registry = {0};
  int value = -1;

  if (!registry_put(&registry, "read", 10) ||
      !registry_put(&registry, "edit", 20) ||
      !registry_put(&registry, "grep", 30)) {
    fprintf(stderr, "FAIL preparacion del registro\n");
    registry_destroy(&registry);
    return 1;
  }
  if (!registry_get(&registry, "edit", &value) || value != 20) {
    fprintf(stderr, "FAIL lookup en cadena\n");
    registry_destroy(&registry);
    return 1;
  }
  value = 99;
  if (registry_get(&registry, "missing", &value) || value != 99 ||
      registry_get(NULL, "read", &value) ||
      registry_get(&registry, NULL, &value) ||
      registry_get(&registry, "read", NULL)) {
    fprintf(stderr, "FAIL clave ausente o NULL safety\n");
    registry_destroy(&registry);
    return 1;
  }

  registry_destroy(&registry);
  return exercise_passed();
}
