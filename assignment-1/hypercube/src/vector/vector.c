#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <float.h>
#include <time.h>

#include "vector.h"
#include "../errors/errors.h"

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

static float* gaussian_rand(float mean, float stddev)
{
	const float epsilon = FLT_EPSILON;
	const float pi = M_PI;

	float r1, r2;
	do {
		r1 = (float)rand()/(float)RAND_MAX;
		r2 = (float)rand()/(float)RAND_MAX;
	} while (r1 <= FLT_EPSILON);

	float mag = stddev * sqrt(-2.0 * log(r1));
	float ndr1 = mag * cos(2 * pi * r2) + mean;
	float ndr2 = mag * sin(2 * pi * r2) + mean;

	float* ndrs = malloc(sizeof(float*) * 2);
	ndrs[0] = ndr1; ndrs[1] = ndr2;

	return ndrs;
}

struct vector* vector_gaussian(unsigned n_dims)
{
	if (!n_dims) { err_code = LSH_INV_ARGS; return NULL; }

	err_code = LSH_OK;

	int* dims = malloc(sizeof(int*) * n_dims);
	if (dims == NULL) { perror("malloc"); exit(1); }
	for (unsigned c_dim = 0; c_dim < n_dims - 1; ++c_dim) {
		float* ndrs = gaussian_rand(0, 1);
		dims[c_dim++] = ndrs[0];
		dims[c_dim] = ndrs[1];
		free(ndrs);
	}
	float* ndrs = gaussian_rand(0, 1);
	dims[n_dims - 1] = ndrs[0];
	free(ndrs);

	return vector_init(0, n_dims, dims);
}

unsigned vector_dist(unsigned norm, struct vector* v1, struct vector* v2)
{
	if (!v1 || !v2) { err_code = LSH_INV_ARGS; return 0; }

	err_code = LSH_OK;

	// The two vectors could be of different dimensions/length.
	unsigned n_dims = (v1->n_dims > v2->n_dims) ? v1->n_dims : v2->n_dims;
	int* v1_dims = calloc(n_dims, sizeof(int));
	memcpy(v1_dims, v1->dims, v1->n_dims);
	int* v2_dims = calloc(n_dims, sizeof(int));
	memcpy(v2_dims, v2->dims, v2->n_dims);

	if (norm == 0) {
		unsigned nonzero = 0;
		for (unsigned c_dim = 0; c_dim < n_dims; ++c_dim)
			if (v1_dims[c_dim] - v2_dims[c_dim] != 0) ++nonzero;
		free(v1_dims); free(v2_dims);
		return nonzero;
	}

	if (norm == 1 || norm == 2) {
		unsigned sum = 0;
		for (unsigned c_dim = 0; c_dim < n_dims; ++c_dim) {
			unsigned term = abs(v1_dims[c_dim] - v2_dims[c_dim]);
			sum += (unsigned)pow(term, (double)1/norm);
		}
		free(v1_dims); free(v2_dims);
		return pow(sum, (double)1/norm);
	}

	if (norm == 3) {
		unsigned max = 0;
		for (unsigned c_dim = 0; c_dim < n_dims; ++c_dim) {
			unsigned term = abs(v1_dims[c_dim] - v2_dims[c_dim]);
			max = (max > term) ? max : term;
		}
		free(v1_dims); free(v2_dims);
		return max;
	}
}

void vector_print(struct vector* vector)
{
	if (vector == NULL) { err_code = LSH_INV_ARGS; return; }

	err_code = LSH_OK;

	printf("id = %u\n", vector->id);
	printf("n_dims = %u\n", vector->n_dims);
	for (unsigned c_dim = 0; c_dim < vector->n_dims; ++c_dim)
		printf("%u ", vector->dims[c_dim]);
	printf("\n");
}

void vector_free(struct vector* vector)
{
	if (vector == NULL) { err_code = LSH_INV_ARGS; return; }

	err_code = LSH_OK;

	free(vector->dims);
	free(vector);
}
