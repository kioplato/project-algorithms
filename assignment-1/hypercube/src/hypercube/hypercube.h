#ifndef LSH_H
#define LSH_H

#include "../hashtable/hashtable.h"

struct hypercube;

/*
 * Initialize LSH data structure.
 *
 * 'n_hfns': number of h functions per hashtable.
 * 'n_hts': number of hashtables.
 *
 * Return value:
 * Success: returns initialized `struct hypercube` and sets `err_code` to `LSH_OK`.
 * Failure: returns NULL and sets `err_code` accordingly.
 */
struct hypercube* hc_init(unsigned n_hfns, unsigned n_hts);

/*
 * Inserts the vectors in the `inputfile`.
 *
 * 'inputfile': path to file with vectors.
 *
 * Return value:
 * Success: returns 1 and sets `err_code` to `LSH_OK`.
 * Failure: returns 0 and sets `err_code` accordingly.
 */
int hc_bulk_insert(char* inputfile);

/*
 * Inserts vector 'vec' into the LSH data structure.
 *
 * 'hc': LSH data structure to insert vector.
 * 'vec': vector to insert.
 *
 * Return value:
 * Success: 1 returned and `err_code` is set to LSH_OK.
 * Failure: 0 returned and `err_code` is set accordingly.
 */
int hc_insert(struct hypercube* hc, struct hc_vector* vec);

#endif  // #ifndef LSH_H
