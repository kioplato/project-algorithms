#include <stdio.h>
#include <stdlib.h>

#include "../errors/errors.h"

struct hypercube {
	unsigned n_hfns;      // Number of h functions per hashtable.
	struct h_fn** h_fns;  // Each hashtable's h functions.

	unsigned n_hts;          // Number of hashtables.
	struct hashtable** hts;  // The hashtables.
};

struct hypercube* hc_init(unsigned n_hfns, unsigned n_hts)
{
	if (!n_hfns || !n_hts) { err_code = LSH_INV_ARGS; return NULL; }
	err_code = LSH_OK;
}

struct hypercube* hc_insert()
{
	hc2_insert();
}
