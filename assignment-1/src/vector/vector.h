#ifndef VECTOR_H
#define VECTOR_H

struct vector {
	unsigned id;  // Vector's id.

	unsigned n_dims;  // Number of dimensions.
	unsigned* dims;   // Dimentions.
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
struct vector* vector_init(unsigned id, unsigned n_dims, unsigned* dims);

/*
 * Free the allocated memory for the `vector` vector.
 *
 * 'vector': vector to free.
 *
 * Success: memory is free'd and `err_code` is set to `LSH_OK`.
 * Failure: `err_code` is set accordingly.
 */
void vector_free(struct vector* vector);

#endif  // #ifndef VECTOR_H
