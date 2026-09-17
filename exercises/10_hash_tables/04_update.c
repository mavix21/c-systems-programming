/*
 * Kata 41: insertar o actualizar
 *
 * Implementa registry_put: actualiza el valor si el nombre ya existe o crea
 * un nodo con una copia propia del nombre. Una actualizacion no cambia length.
 */

#include <stdint.h>
#include <stdlib.h>
#include <string.h>

#include "exercise.h"

#define BUCKET_COUNT 8

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

static int registry_put(struct registry *registry, const char *name,
                        int value) {
  (void)registry;
  (void)name;
  (void)value;

  /* TODO: actualiza o inserta una copia hecha con malloc/memcpy. */
  return 0;
}

int main(void) {
  struct registry registry = {0};
  char source[] = "read";
  int value;

  if (!registry_put(&registry, source, 1)) {
    fprintf(stderr, "FAIL insercion inicial\n");
    registry_destroy(&registry);
    return 1;
  }
  source[0] = 'X';
  if (!registry_get(&registry, "read", &value) || value != 1) {
    fprintf(stderr, "FAIL copia propia de la clave\n");
    registry_destroy(&registry);
    return 1;
  }
  if (!registry_put(&registry, "read", 7) || registry.length != 1 ||
      !registry_get(&registry, "read", &value) || value != 7) {
    fprintf(stderr, "FAIL update sin duplicado\n");
    registry_destroy(&registry);
    return 1;
  }

  registry_destroy(&registry);
  return exercise_passed();
}
