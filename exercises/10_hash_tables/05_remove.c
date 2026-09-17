/*
 * Kata 42: eliminar de una tabla hash
 *
 * Implementa registry_remove con separate chaining. Reconecta la cadena,
 * libera nombre y nodo, y reduce length solo cuando encuentra la herramienta.
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
  for (node = registry->buckets[bucket]; node != NULL; node = node->next) {
    if (strcmp(node->name, name) == 0) {
      node->value = value;
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
  node->value = value;
  node->next = registry->buckets[bucket];
  registry->buckets[bucket] = node;
  registry->length++;
  return 1;
}

static int registry_get(const struct registry *registry, const char *name,
                        int *out_value) {
  struct registry_node *node;

  if (registry == NULL || name == NULL || out_value == NULL)
    return 0;
  node = registry->buckets[hash_string(name) % BUCKET_COUNT];
  while (node != NULL) {
    if (strcmp(node->name, name) == 0) {
      *out_value = node->value;
      return 1;
    }
    node = node->next;
  }
  return 0;
}

static int registry_remove(struct registry *registry, const char *name) {
  (void)registry;
  (void)name;

  /* TODO: elimina el nodo usando current y previous (o pointer-to-pointer). */
  return 0;
}

int main(void) {
  struct registry registry = {0};
  int value;

  if (!registry_put(&registry, "read", 1) ||
      !registry_put(&registry, "edit", 2) ||
      !registry_put(&registry, "grep", 3)) {
    fprintf(stderr, "FAIL preparacion del registro\n");
    registry_destroy(&registry);
    return 1;
  }
  if (!registry_remove(&registry, "grep") || registry.length != 2 ||
      registry_get(&registry, "grep", &value)) {
    fprintf(stderr, "FAIL eliminar cabeza del bucket\n");
    registry_destroy(&registry);
    return 1;
  }
  if (!registry_remove(&registry, "read") || registry.length != 1 ||
      registry_get(&registry, "read", &value) ||
      !registry_get(&registry, "edit", &value) || value != 2) {
    fprintf(stderr, "FAIL eliminar dentro de la cadena\n");
    registry_destroy(&registry);
    return 1;
  }
  if (registry_remove(&registry, "missing") || registry.length != 1 ||
      registry_remove(NULL, "edit") || registry_remove(&registry, NULL)) {
    fprintf(stderr, "FAIL clave ausente o NULL safety\n");
    registry_destroy(&registry);
    return 1;
  }

  registry_destroy(&registry);
  return exercise_passed();
}
