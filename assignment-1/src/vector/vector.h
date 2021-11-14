#ifndef VECTOR_H
#define VECTOR_H

struct vector {
	unsigned n_dims;  // Number of dimensions.
	int* dims;   // Dimentions.
};

/*
 * Create a new vector with the specified dimensions.
 *
 * 'id': ID or unique key of the vector.
 * 'n_dims': number of dimensions of the vector.
 * 'dims': dimensions of the vector.
 *
 * Return value:
 * Success: heap allocated `struct vector` structure with specified members.
 * Failure: NULL is returned and `err_code` is set accordingly.
 */
struct vector* vector_init(unsigned n_dims, int* dims);

/*
 * Create a multivariate gaussian distribution.
 * Does not seed `srand()`.
 *
 * 'n_dims': number of dimensions to create.
 *
 * Return value:
 * Success: returns gaussian vector `struct vector`.
 * Failure: NULL and `err_code` is set accordingly.
 */
struct vector* vector_gaussian(unsigned n_dims);

/*
 * Distance between two vectors.
 *
 * 'norm': metric function.
 * 'vec1': first vector.
 * 'vec2': second vector.
 *
 * Possible values of norm:
 *   0 = Number of non-zero entries
 *   1 = Manhattan distance
 *   2 = Euclidean distance
 *   3 = Max Distance
 *
 * Return value:
 * Success: returns the distance of the two vectors.
 * Failure: returns 0 and sets `err_code`.
 */
unsigned vector_dist(unsigned norm, struct vector* vec1, struct vector* vec2);

/*
 * Print the given vector `vector`.
 *
 * Return value:
 * Success: prints the vector and sets `err_code` to `LSH_OK`.
 * Failure: sets `err_code` to `LSH_INV_ARGS`.
 */
void vector_print(struct vector* vector);

/*
 * Free the allocated memory for the `vector` vector.
 *
 * 'vector': vector to free.
 *
 * Return value:
 * Success: memory is free'd and `err_code` is set to `LSH_OK`.
 * Failure: `err_code` is set accordingly.
 */
void vector_free(struct vector* vector);

#endif  // #ifndef VECTOR_H
