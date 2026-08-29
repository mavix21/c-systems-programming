#ifndef EXERCISE_H
#define EXERCISE_H

#include <stddef.h>
#include <stdio.h>

#define CHECK_INT(actual, expected)                                           \
	do {                                                                     \
		int actual_value = (actual);                                       \
		int expected_value = (expected);                                   \
		if (actual_value != expected_value) {                              \
			fprintf(stderr, "FAIL %s:%d: esperado %d, obtenido %d\n",   \
				__FILE__, __LINE__, expected_value, actual_value);      \
			return 1;                                                    \
		}                                                                    \
	} while (0)

#define CHECK_SIZE(actual, expected)                                          \
	do {                                                                     \
		size_t actual_value = (actual);                                    \
		size_t expected_value = (expected);                                \
		if (actual_value != expected_value) {                              \
			fprintf(stderr,                                               \
				"FAIL %s:%d: esperado %zu, obtenido %zu\n",          \
				__FILE__, __LINE__, expected_value, actual_value);      \
			return 1;                                                    \
		}                                                                    \
	} while (0)

static inline int exercise_passed(void)
{
	puts("PASS");
	return 0;
}

#endif
