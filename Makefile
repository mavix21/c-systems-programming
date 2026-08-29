CC := clang
CFLAGS := -std=c17 -Wall -Wextra -Werror -pedantic -g
SANITIZER_FLAGS := -fsanitize=address,undefined -fno-omit-frame-pointer

EXERCISE ?= 00_fundamentals/01_temperature
SOURCE := exercises/$(EXERCISE).c
BINARY := bin/$(EXERCISE)
SOURCES := $(sort $(wildcard exercises/*/*.c))
EXAMPLE ?= 05_memory/06_matrix_contiguous
EXAMPLE_SOURCE := examples/$(EXAMPLE).c
EXAMPLE_BINARY := bin/examples/$(EXAMPLE)

.DEFAULT_GOAL := help

.PHONY: help list build run check verify sanitize debug leaks run-example clean

help:
	@printf '%s\n' \
		'C playground' \
		'' \
		'make list                              Lista las katas' \
		'make run EXERCISE=00_fundamentals/01_temperature' \
		'make build EXERCISE=01_pointers/01_swap' \
		'make sanitize EXERCISE=05_memory/01_range' \
		'make debug EXERCISE=01_pointers/01_swap' \
		'make leaks EXERCISE=05_memory/01_range' \
		'make run-example EXAMPLE=05_memory/06_matrix_contiguous' \
		'make verify                            Compila todas las katas' \
		'make clean'

list:
	@for source in $(SOURCES); do \
		printf '%s\n' "$${source#exercises/}" | sed 's/\.c$$//'; \
	done

build:
	@test -f "$(SOURCE)" || { printf 'No existe: %s\n' "$(SOURCE)"; exit 1; }
	@mkdir -p "$(dir $(BINARY))"
	$(CC) $(CFLAGS) -Iinclude "$(SOURCE)" -o "$(BINARY)"

run check: build
	@"$(BINARY)"

verify:
	@mkdir -p bin/verify
	@for source in $(SOURCES); do \
		name=$${source#exercises/}; \
		name=$${name%.c}; \
		output="bin/verify/$$(printf '%s' "$$name" | tr '/' '_')"; \
		printf 'Compilando %s... ' "$$name"; \
		if $(CC) $(CFLAGS) -Iinclude "$$source" -o "$$output"; then \
			printf 'OK\n'; \
		else \
			exit 1; \
		fi; \
	done

sanitize:
	@test -f "$(SOURCE)" || { printf 'No existe: %s\n' "$(SOURCE)"; exit 1; }
	@mkdir -p "$(dir $(BINARY))"
	$(CC) $(CFLAGS) $(SANITIZER_FLAGS) -Iinclude "$(SOURCE)" -o "$(BINARY)"
	@"$(BINARY)"

debug: build
	lldb "$(BINARY)"

leaks: build
	leaks --atExit -- "$(BINARY)"

run-example:
	@test -f "$(EXAMPLE_SOURCE)" || { printf 'No existe: %s\n' "$(EXAMPLE_SOURCE)"; exit 1; }
	@mkdir -p "$(dir $(EXAMPLE_BINARY))"
	$(CC) $(CFLAGS) $(SANITIZER_FLAGS) -Iinclude "$(EXAMPLE_SOURCE)" -o "$(EXAMPLE_BINARY)"
	@"$(EXAMPLE_BINARY)"

clean:
	rm -rf bin
