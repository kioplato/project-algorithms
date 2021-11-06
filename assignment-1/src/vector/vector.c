#include <stdio.h>
#include <stdlib.h>

#include "vector.h"
//#include "../errors/errors.h"
#include "errors.h"

struct vector* vector_init(unsigned id, unsigned n_dims, unsigned* dims)
{
	if (!n_dims || !dims) { err_code = LSH_INV_ARGS; return NULL; }

	err_code = LSH_OK;

	struct vector* vector = malloc(sizeof(struct vector));
	if (vector == NULL) { perror("malloc"); exit(1); }

	vector->id = id;
	vector->n_dims = n_dims;
	vector->dims = dims;

	return vector;
}

void vector_free(struct vector* vector)
{
	if (vector == NULL) { err_code = LSH_INV_ARGS; return; }

	err_code = LSH_OK;

	free(vector->dims);
	free(vector);

	return;
}
