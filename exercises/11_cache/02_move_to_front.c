/*
 * Kata 46: mover una entrada al frente
 *
 * La cache ordena archivos desde MRU (head) hasta LRU (tail). Implementa
 * cache_touch para separar una entrada existente y convertirla en el MRU.
 */

#include <stdlib.h>
#include <string.h>

#include "exercise.h"

struct cache_entry {
  char *path;
  char *content;
  struct cache_entry *newer;
  struct cache_entry *older;
};

struct cache {
  struct cache_entry *head;
  struct cache_entry *tail;
  size_t length;
};

static char *copy_string(const char *text) {
  size_t size = strlen(text) + 1;
  char *copy = malloc(size);

  if (copy != NULL)
    memcpy(copy, text, size);
  return copy;
}

static int cache_add_fixture(struct cache *cache, const char *path,
                             const char *content) {
  struct cache_entry *entry = calloc(1, sizeof(*entry));

  if (entry == NULL)
    return 0;
  entry->path = copy_string(path);
  entry->content = copy_string(content);
  if (entry->path == NULL || entry->content == NULL) {
    free(entry->path);
    free(entry->content);
    free(entry);
    return 0;
  }

  entry->older = cache->head;
  if (cache->head != NULL)
    cache->head->newer = entry;
  else
    cache->tail = entry;
  cache->head = entry;
  cache->length++;
  return 1;
}

static void cache_destroy(struct cache *cache) {
  struct cache_entry *entry;

  if (cache == NULL)
    return;
  entry = cache->head;
  while (entry != NULL) {
    struct cache_entry *older = entry->older;

    free(entry->path);
    free(entry->content);
    free(entry);
    entry = older;
  }
  *cache = (struct cache){0};
}

static void cache_touch(struct cache *cache, struct cache_entry *entry) {
  /* TODO: separa entry de su posicion y muevela a head. */
  (void)cache;
  (void)entry;
}

static int cache_has_order(const struct cache *cache, const char *first,
                           const char *second, const char *third) {
  const struct cache_entry *entry = cache->head;
  const struct cache_entry *newer = NULL;
  const char *expected[] = {first, second, third};
  size_t count = 0;

  while (entry != NULL && count < 3) {
    if (entry->newer != newer || strcmp(entry->path, expected[count]) != 0)
      return 0;
    newer = entry;
    entry = entry->older;
    count++;
  }
  return entry == NULL && count == 3 && newer == cache->tail &&
         cache->head->newer == NULL && cache->tail->older == NULL &&
         cache->length == 3;
}

int main(void) {
  struct cache cache = {0};
  struct cache_entry *middle;
  struct cache_entry *tail;

  if (!cache_add_fixture(&cache, "src/main.c", "main") ||
      !cache_add_fixture(&cache, "AGENTS.md", "agent rules") ||
      !cache_add_fixture(&cache, ".cache/tool-output.txt", "tool output")) {
    fprintf(stderr, "FAIL preparacion de la cache\n");
    cache_destroy(&cache);
    return 1;
  }

  middle = cache.head->older;
  cache_touch(&cache, middle);
  if (!cache_has_order(&cache, "AGENTS.md", ".cache/tool-output.txt",
                       "src/main.c")) {
    fprintf(stderr, "FAIL mover desde el medio\n");
    cache_destroy(&cache);
    return 1;
  }

  tail = cache.tail;
  cache_touch(&cache, tail);
  if (!cache_has_order(&cache, "src/main.c", "AGENTS.md",
                       ".cache/tool-output.txt")) {
    fprintf(stderr, "FAIL mover tail\n");
    cache_destroy(&cache);
    return 1;
  }

  cache_touch(&cache, cache.head);
  if (!cache_has_order(&cache, "src/main.c", "AGENTS.md",
                       ".cache/tool-output.txt")) {
    fprintf(stderr, "FAIL tocar head\n");
    cache_destroy(&cache);
    return 1;
  }

  cache_destroy(&cache);
  return exercise_passed();
}
