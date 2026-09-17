/*
 * Kata 47: lookup LRU con hash
 *
 * Cada entrada pertenece a un bucket y a la lista de recencia. Implementa
 * cache_get: busca path, promueve un hit a MRU y retorna content prestado.
 * Un miss retorna NULL y no modifica la cache.
 */

#include <stdlib.h>
#include <string.h>

#include "exercise.h"

#define CACHE_BUCKETS 5

struct cache_entry {
  char *path;
  char *content;
  struct cache_entry *hash_next;
  struct cache_entry *newer;
  struct cache_entry *older;
};

struct cache {
  struct cache_entry *buckets[CACHE_BUCKETS];
  struct cache_entry *head;
  struct cache_entry *tail;
  size_t length;
};

static size_t cache_hash(const char *path) {
  size_t hash = 5381;

  while (*path != '\0') {
    hash = hash * 33u + (unsigned char)*path;
    path++;
  }
  return hash % CACHE_BUCKETS;
}

static char *copy_string(const char *text) {
  size_t size = strlen(text) + 1;
  char *copy = malloc(size);

  if (copy != NULL)
    memcpy(copy, text, size);
  return copy;
}

static void cache_link_front(struct cache *cache, struct cache_entry *entry) {
  entry->newer = NULL;
  entry->older = cache->head;
  if (cache->head != NULL)
    cache->head->newer = entry;
  else
    cache->tail = entry;
  cache->head = entry;
}

static int cache_insert_fixture(struct cache *cache, const char *path,
                                const char *content) {
  struct cache_entry *entry = calloc(1, sizeof(*entry));
  size_t bucket;

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

  bucket = cache_hash(path);
  entry->hash_next = cache->buckets[bucket];
  cache->buckets[bucket] = entry;
  cache_link_front(cache, entry);
  cache->length++;
  return 1;
}

static struct cache_entry *cache_find(struct cache *cache, const char *path) {
  struct cache_entry *entry;

  if (cache == NULL || path == NULL)
    return NULL;
  entry = cache->buckets[cache_hash(path)];
  while (entry != NULL) {
    if (strcmp(entry->path, path) == 0)
      return entry;
    entry = entry->hash_next;
  }
  return NULL;
}

static const char *cache_get(struct cache *cache, const char *path) {
  struct cache_entry *entry = cache_find(cache, path);

  /* TODO: si hay un hit, muevelo a head y retorna su content prestado. */
  (void)entry;
  return NULL;
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

int main(void) {
  struct cache cache = {0};
  struct cache_entry *head;
  struct cache_entry *tail;
  const char *content;

  if (!cache_insert_fixture(&cache, "src/main.c", "main source") ||
      !cache_insert_fixture(&cache, "AGENTS.md", "workspace rules") ||
      !cache_insert_fixture(&cache, ".cache/tool-output.txt", "tool output")) {
    fprintf(stderr, "FAIL preparacion de la cache\n");
    cache_destroy(&cache);
    return 1;
  }

  head = cache.head;
  tail = cache.tail;
  if (cache_get(&cache, "missing.c") != NULL || cache.head != head ||
      cache.tail != tail || cache.length != 3) {
    fprintf(stderr, "FAIL miss modifico la cache\n");
    cache_destroy(&cache);
    return 1;
  }

  content = cache_get(&cache, "src/main.c");
  if (content == NULL || strcmp(content, "main source") != 0 ||
      strcmp(cache.head->path, "src/main.c") != 0 ||
      strcmp(cache.tail->path, "AGENTS.md") != 0 ||
      cache.head->newer != NULL || cache.tail->older != NULL) {
    fprintf(stderr, "FAIL promocion desde LRU\n");
    cache_destroy(&cache);
    return 1;
  }

  content = cache_get(&cache, "AGENTS.md");
  if (content == NULL || strcmp(content, "workspace rules") != 0 ||
      strcmp(cache.head->path, "AGENTS.md") != 0 ||
      cache.head->older->newer != cache.head || cache.length != 3) {
    fprintf(stderr, "FAIL promocion desde el medio\n");
    cache_destroy(&cache);
    return 1;
  }

  cache_destroy(&cache);
  return exercise_passed();
}
