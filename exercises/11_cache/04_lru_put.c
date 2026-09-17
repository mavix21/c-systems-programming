/*
 * Kata 48: insertar y actualizar en una cache LRU
 *
 * Implementa cache_put con copias propias de path y content. Un path nuevo se
 * agrega como MRU; una actualizacion reemplaza content, promueve la entrada y
 * no cambia length. Los fallos de reserva deben dejar una cache valida.
 */

#include <stdlib.h>
#include <string.h>

#include "exercise.h"

#define CACHE_BUCKETS 5
#define CACHE_CAPACITY 4

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

static int cache_put(struct cache *cache, const char *path,
                     const char *content) {
  struct cache_entry *entry;

  if (cache == NULL || path == NULL || content == NULL || cache->capacity == 0)
    return 0;

  entry = cache_find(cache, path);
  if (entry == NULL && cache->length >= cache->capacity)
    return 0;

  /* TODO: publica una entrada nueva o reemplaza content de forma atomica. */
  (void)entry;
  return 0;
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
  char path[] = "src/main.c";
  char content[] = "first version";
  char updated[] = "second version";
  struct cache_entry *main_entry;

  if (!cache_put(&cache, path, content) ||
      !cache_put(&cache, "AGENTS.md", "workspace rules") ||
      !cache_put(&cache, ".cache/tool-output.txt", "tool output")) {
    fprintf(stderr, "FAIL insercion\n");
    cache_destroy(&cache);
    return 1;
  }
  path[0] = 'X';
  content[0] = 'X';
  if (cache_get(&cache, "src/main.c") == NULL)
    main_entry = NULL;
  else
    main_entry = cache_find(&cache, "src/main.c");
  if (main_entry == NULL || strcmp(main_entry->content, "first version") != 0 ||
      cache.head != main_entry || cache.length != 3) {
    fprintf(stderr, "FAIL ownership de la insercion\n");
    cache_destroy(&cache);
    return 1;
  }

  if (!cache_put(&cache, "src/main.c", updated)) {
    fprintf(stderr, "FAIL actualizacion\n");
    cache_destroy(&cache);
    return 1;
  }
  updated[0] = 'X';
  main_entry = cache_find(&cache, "src/main.c");
  if (main_entry == NULL || strcmp(main_entry->content, "second version") != 0 ||
      cache.head != main_entry || cache.length != 3 ||
      cache.head->newer != NULL || cache.tail->older != NULL) {
    fprintf(stderr, "FAIL contenido o recencia de la actualizacion\n");
    cache_destroy(&cache);
    return 1;
  }

  cache_destroy(&cache);
  return exercise_passed();
}
