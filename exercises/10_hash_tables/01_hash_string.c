/*
 * Kata 38: hash determinista de strings
 *
 * Implementa FNV-1a de 32 bits para nombres de herramientas. Procesa cada
 * byte como unsigned char para que el resultado no dependa de la plataforma.
 */

#include <inttypes.h>
#include <stdint.h>

#include "exercise.h"

static uint32_t hash_string(const char *text) {
  (void)text;

  /* TODO: aplica FNV-1a desde el offset basis hasta el byte '\0'. */
  return 0;
}

int main(void) {
  const uint32_t empty_hash = UINT32_C(2166136261);
  const uint32_t a_hash = UINT32_C(0xe40c292c);
  uint32_t read_hash;
  uint32_t edit_hash;
  uint32_t grep_hash;

  if (hash_string("") != empty_hash || hash_string("a") != a_hash) {
    fprintf(stderr, "FAIL hashes conocidos: 0x%08" PRIx32 " 0x%08" PRIx32
                    "\n",
            hash_string(""), hash_string("a"));
    return 1;
  }

  read_hash = hash_string("read");
  edit_hash = hash_string("edit");
  grep_hash = hash_string("grep");
  if (read_hash != hash_string("read") || read_hash == edit_hash ||
      edit_hash == grep_hash || read_hash == grep_hash) {
    fprintf(stderr, "FAIL estabilidad o dispersion de comandos\n");
    return 1;
  }

  return exercise_passed();
}
