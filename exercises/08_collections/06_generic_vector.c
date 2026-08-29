/*
 * Kata 29: vector generico
 *
 * Almacena elementos de cualquier tipo como bytes. Implementa vector_push
 * copiando element_size bytes. No guardes el puntero recibido: copia su valor.
 */

#include <stdint.h>
#include <stdlib.h>
#include <string.h>

#include "exercise.h"

struct vector {
	void *items;
	size_t element_size;
	size_t length;
	size_t capacity;
};

struct point {
	int x;
	int y;
};

static void vector_destroy(struct vector *vector)
{
	if (vector == NULL)
		return;
	free(vector->items);
	*vector = (struct vector){0};
}

static int vector_reserve(struct vector *vector, size_t additional)
{
	size_t required;
	size_t capacity;
	void *resized;

	if (additional > SIZE_MAX - vector->length)
		return 0;
	required = vector->length + additional;
	if (required <= vector->capacity)
		return 1;
	capacity = vector->capacity == 0 ? 4 : vector->capacity;
	while (capacity < required) {
		if (capacity > SIZE_MAX / 2) {
			capacity = required;
			break;
		}
		capacity *= 2;
	}
	if (vector->element_size == 0 || capacity > SIZE_MAX / vector->element_size)
		return 0;
	resized = realloc(vector->items, capacity * vector->element_size);
	if (resized == NULL)
		return 0;
	vector->items = resized;
	vector->capacity = capacity;
	return 1;
}

static void *vector_at(struct vector *vector, size_t index)
{
	if (index >= vector->length)
		return NULL;
	return (unsigned char *)vector->items + index * vector->element_size;
}

static int vector_push(struct vector *vector, const void *element)
{
	/* TODO: reserva y copia element al offset indicado por length. */
	if (!vector_reserve(vector, 1))
		return 0;
	(void)element;
	return 0;
}

int main(void)
{
	struct vector vector = {.element_size = sizeof(struct point)};
	struct point first = {10, 20};
	struct point second = {30, 40};
	struct point *stored;

	if (!vector_push(&vector, &first) || !vector_push(&vector, &second)) {
		fprintf(stderr, "FAIL vector_push generico\n");
		vector_destroy(&vector);
		return 1;
	}
	first.x = -1;
	stored = vector_at(&vector, 0);
	if (stored == NULL || stored->x != 10 || stored->y != 20 ||
		vector_at(&vector, 2) != NULL) {
		fprintf(stderr, "FAIL copia o acceso generico\n");
		vector_destroy(&vector);
		return 1;
	}

	vector_destroy(&vector);
	return exercise_passed();
}
