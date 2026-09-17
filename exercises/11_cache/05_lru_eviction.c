/*
 * Kata 49: eviction LRU
 *
 * La cache ya inserta, actualiza y promueve entradas. Completa la expulsion:
 * al insertar con capacidad llena, elimina tail de su bucket y de la lista,
 * libera su ownership y deja sitio para el nuevo archivo del workspace.
 */

#include <stdlib.h>
#include <string.h>

#include "exercise.h"

#define CACHE_BUCKETS 5
#define CACHE_CAPACITY 3

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
  size_t capacity;
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

static void cache_link_front(struct cache *cache, struct cache_entry *entry) {
  entry->newer = NULL;
  entry->older = cache->head;
  if (cache->head != NULL)
    cache->head->newer = entry;
  else
    cache->tail = entry;
  cache->head = entry;
}

static void cache_touch(struct cache *cache, struct cache_entry *entry) {
  if (entry == cache->head)
    return;
  if (entry->newer != NULL)
    entry->newer->older = entry->older;
  if (entry->older != NULL)
    entry->older->newer = entry->newer;
  else
    cache->tail = entry->newer;
  cache_link_front(cache, entry);
}

static int cache_evict_lru(struct cache *cache) {
  /* TODO: elimina tail del bucket y de la lista, y libera la entrada. */
  (void)cache;
  return 0;
}

static int cache_put(struct cache *cache, const char *path,
                     const char *content) {
  struct cache_entry *entry;
  char *new_path;
  char *new_content;
  size_t bucket;

  if (cache == NULL || path == NULL || content == NULL || cache->capacity == 0)
    return 0;

  entry = cache_find(cache, path);
  if (entry != NULL) {
    new_content = copy_string(content);
    if (new_content == NULL)
      return 0;
    free(entry->content);
    entry->content = new_content;
    cache_touch(cache, entry);
    return 1;
  }

  entry = calloc(1, sizeof(*entry));
  if (entry == NULL)
    return 0;
  new_path = copy_string(path);
  new_content = copy_string(content);
  if (new_path == NULL || new_content == NULL) {
    free(new_path);
    free(new_content);
    free(entry);
    return 0;
  }
  entry->path = new_path;
  entry->content = new_content;

  if (cache->length == cache->capacity && !cache_evict_lru(cache)) {
    free(entry->path);
    free(entry->content);
    free(entry);
    return 0;
  }

  bucket = cache_hash(entry->path);
  entry->hash_next = cache->buckets[bucket];
  cache->buckets[bucket] = entry;
  cache_link_front(cache, entry);
  cache->length++;
  return 1;
}

static const char *cache_get(struct cache *cache, const char *path) {
  struct cache_entry *entry = cache_find(cache, path);

  if (entry == NULL)
    return NULL;
  cache_touch(cache, entry);
  return entry->content;
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
  struct cache cache = {.capacity = CACHE_CAPACITY};
  const char *content;

  if (!cache_put(&cache, "src/main.c", "main source") ||
      !cache_put(&cache, "AGENTS.md", "workspace rules") ||
      !cache_put(&cache, ".cache/tool-output.txt", "tool output")) {
    fprintf(stderr, "FAIL llenar la cache\n");
    cache_destroy(&cache);
    return 1;
  }

  content = cache_get(&cache, "src/main.c");
  if (content == NULL || strcmp(content, "main source") != 0 ||
      strcmp(cache.head->path, "src/main.c") != 0 ||
      strcmp(cache.tail->path, "AGENTS.md") != 0) {
    fprintf(stderr, "FAIL lookup y promocion MRU\n");
    cache_destroy(&cache);
    return 1;
  }

  if (!cache_put(&cache, "src/cache.c", "new workspace file")) {
    fprintf(stderr, "FAIL eviction e insercion\n");
    cache_destroy(&cache);
    return 1;
  }
  if (cache.length != CACHE_CAPACITY || cache_find(&cache, "AGENTS.md") != NULL ||
      cache_find(&cache, "src/cache.c") != cache.head ||
      strcmp(cache.tail->path, ".cache/tool-output.txt") != 0 ||
      cache.head->newer != NULL || cache.tail->older != NULL) {
    fprintf(stderr, "FAIL victima LRU o invariantes\n");
    cache_destroy(&cache);
    return 1;
  }

  content = cache_get(&cache, ".cache/tool-output.txt");
  if (content == NULL || strcmp(content, "tool output") != 0 ||
      strcmp(cache.head->path, ".cache/tool-output.txt") != 0) {
    fprintf(stderr, "FAIL lookup despues de eviction\n");
    cache_destroy(&cache);
    return 1;
  }

  cache_destroy(&cache);
  if (cache.head != NULL || cache.tail != NULL || cache.length != 0 ||
      cache.capacity != 0) {
    fprintf(stderr, "FAIL limpieza\n");
    return 1;
  }
  return exercise_passed();
}
