/*
 * Kata 45: lista doble de recencia
 *
 * head es el archivo mas reciente (MRU) y tail el menos reciente (LRU).
 * newer apunta hacia head; older apunta hacia tail. Enlaza una entrada
 * separada al frente, tanto en una cache vacia como en una no vacia.
 */

#include "exercise.h"

struct cache_entry {
  const char *path;
  const char *content;
  struct cache_entry *newer;
  struct cache_entry *older;
};

struct cache {
  struct cache_entry *head;
  struct cache_entry *tail;
  size_t length;
};

static void cache_link_front(struct cache *cache, struct cache_entry *entry) {
  /* TODO: enlaza entry, que llega separada, como nuevo MRU. */
  (void)cache;
  (void)entry;
}

int main(void) {
  struct cache empty = {0};
  struct cache_entry main_file = {
      .path = "src/main.c", .content = "int main(void) { return 0; }"};
  struct cache_entry agents = {
      .path = "AGENTS.md", .content = "Run clang before replying."};
  struct cache_entry tool_output = {
      .path = ".cache/tool-output.txt", .content = "5 tests passed"};
  struct cache populated = {
      .head = &agents, .tail = &agents, .length = 1};

  cache_link_front(&empty, &main_file);
  if (empty.head != &main_file || empty.tail != &main_file ||
      empty.length != 1 || main_file.newer != NULL ||
      main_file.older != NULL) {
    fprintf(stderr, "FAIL enlace en cache vacia\n");
    return 1;
  }

  cache_link_front(&populated, &tool_output);
  if (populated.head != &tool_output || populated.tail != &agents ||
      populated.length != 2 || tool_output.newer != NULL ||
      tool_output.older != &agents || agents.newer != &tool_output ||
      agents.older != NULL) {
    fprintf(stderr, "FAIL enlace delante de una entrada\n");
    return 1;
  }

  return exercise_passed();
}
