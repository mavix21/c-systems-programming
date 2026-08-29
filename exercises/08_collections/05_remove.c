/*
 * Kata 28: eliminar y compactar
 *
 * Elimina el elemento de index, lo guarda en out_value y compacta el vector.
 * Retorna 0 si index esta fuera de rango. No reduzcas capacity.
 */

#include <stdlib.h>
#include <string.h>

#include "exercise.h"

struct int_vector {
	int *items;
	size_t length;
	size_t capacity;
};

static void vector_destroy(struct int_vector *vector)
{
	if (vector == NULL)
		return;
	free(vector->items);
	*vector = (struct int_vector){0};
}

static int vector_remove(struct int_vector *vector, size_t index,
	int *out_value)
{
	/* TODO: valida, guarda, desplaza a la izquierda y reduce length. */
	(void)vector;
	(void)index;
	(void)out_value;
	return 0;
}

int main(void)
{
	struct int_vector vector = {0};
	int removed = 0;

	vector.items = malloc(4 * sizeof(*vector.items));
	if (vector.items == NULL)
		return 1;
	vector.items[0] = 10;
	vector.items[1] = 20;
	vector.items[2] = 30;
	vector.items[3] = 40;
	vector.length = 4;
	vector.capacity = 4;

	if (!vector_remove(&vector, 1, &removed) || removed != 20) {
		fprintf(stderr, "FAIL vector_remove\n");
		vector_destroy(&vector);
		return 1;
	}
	if (vector.length != 3 || vector.capacity != 4 || vector.items[0] != 10 ||
		vector.items[1] != 30 || vector.items[2] != 40) {
		fprintf(stderr, "FAIL estado posterior a remove\n");
		vector_destroy(&vector);
		return 1;
	}

	vector_destroy(&vector);
	return exercise_passed();
}
