/*
 * Kata 30: coleccion con ownership profundo
 *
 * push_copy guarda una copia heap de text. La lista es propietaria de cada
 * copia y debe liberar primero las strings y despues el array de punteros.
 */

#include <stdint.h>
#include <stdlib.h>
#include <string.h>

#include "exercise.h"

struct string_list {
	char **items;
	size_t length;
	size_t capacity;
};

static void string_list_destroy(struct string_list *list)
{
	if (list == NULL)
		return;
	for (size_t i = 0; i < list->length; i++)
		free(list->items[i]);
	free(list->items);
	*list = (struct string_list){0};
}

static int string_list_reserve(struct string_list *list, size_t additional)
{
	size_t required;
	size_t capacity;
	char **resized;

	if (additional > SIZE_MAX - list->length)
		return 0;
	required = list->length + additional;
	if (required <= list->capacity)
		return 1;
	capacity = list->capacity == 0 ? 4 : list->capacity;
	while (capacity < required) {
		if (capacity > SIZE_MAX / 2) {
			capacity = required;
			break;
		}
		capacity *= 2;
	}
	if (capacity > SIZE_MAX / sizeof(*resized))
		return 0;
	resized = realloc(list->items, capacity * sizeof(*resized));
	if (resized == NULL)
		return 0;
	list->items = resized;
	list->capacity = capacity;
	return 1;
}

static int string_list_push_copy(struct string_list *list, const char *text)
{
	/* TODO: reserva un slot, duplica text y aumenta length solo al completar. */
	if (!string_list_reserve(list, 1))
		return 0;
	(void)text;
	return 0;
}

int main(void)
{
	struct string_list list = {0};
	char source[] = "skill.md";

	if (!string_list_push_copy(&list, source) ||
		!string_list_push_copy(&list, "config.json")) {
		fprintf(stderr, "FAIL string_list_push_copy\n");
		string_list_destroy(&list);
		return 1;
	}
	source[0] = 'X';
	if (list.length != 2 || strcmp(list.items[0], "skill.md") != 0 ||
		strcmp(list.items[1], "config.json") != 0) {
		fprintf(stderr, "FAIL ownership de strings\n");
		string_list_destroy(&list);
		return 1;
	}

	string_list_destroy(&list);
	return exercise_passed();
}
