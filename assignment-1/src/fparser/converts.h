#ifndef CONVERTS_H
#define CONVERTS_H

//#include "../vector/vector.h"
#include "vector.h"

void* convert_to_string(const void**, void* token)
{
	unsigned token_len = *(unsigned*)token;
	char* conv = malloc(token_len + 1);
	memcpy(conv, token + sizeof(unsigned), token_len);
	conv[token_len] = '\0';

	return conv;
}

static int is_positive_integer(char* str)
{
	for (; *str != '\0'; ++str)
		if (*str < '0' || *str > '9') return 0;

	return 1;
}

void* convert_to_vector(const void** mem, void* token)
{
	unsigned token_len = *(unsigned*)token;
	char* strvec = token + sizeof(unsigned);
	strvec[token_len - 1] = '\0';  // Replace the new line.

	/* `strtok()` the string. */
	strtok(strvec, " ");
	char* strvectok = NULL;

	// Read the vector's ID.
	if ((strvectok = strtok(NULL, " ")) != NULL) {
		if (!is_positive_integer(strvectok)) {
			printf("Error: ignoring invalid vector %s\n", strvec);
			printf("Error: invalid vector's token %s\n", strvectok);
			return NULL;  // Invalid vector.
		}
	} else return NULL;  // Empty line.
	unsigned id = atoi(strvectok);

	// Read the vector's dimensions.
	unsigned n_dims = 8;
	unsigned c_dims = 0;
	unsigned* dims = malloc(n_dims * sizeof(unsigned));
	while ((strvectok = strtok(NULL, " ")) != NULL) {
		if (!is_positive_integer(strvectok)) {
			printf("Error: ignoring invalid vector %s\n", strvec);
			printf("Error: invalid vector's token %s\n", strvectok);
			free(dims); return NULL;  // Invalid vector.
		}

		dims[c_dims++] = atoi(strvectok);
		if (c_dims == n_dims) {
			dims = realloc(dims, (n_dims *= 2) * sizeof(unsigned));
			if (dims == NULL) { perror("realloc"); exit(1); }
		}
	}

	// Compare number of dimensions with the first vector (if exists).
	unsigned n_vectors = *(unsigned*)mem;
	if (n_vectors != 0 && c_dims != (*(mem + sizeof(unsigned)))->n_dims) {
		printf("Error: ignoring invalid vector %s\n", strvec);
		printf("Error: different number of dimensions\n");
		free(dims); return NULL;
	}
	return vector_init(id, c_dims, dims);
}

#endif  // #ifndef CONVERTS_H
