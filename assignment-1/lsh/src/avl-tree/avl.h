#ifndef AVL_H
#define AVL_H

// TODO: Implement function for deleting a value.

struct avl_node;

struct avl_tree {
	struct avl_node* root;  // The root of the AVL tree.

	unsigned n_nodes;  // Number of nodes stored.

	/*
	 * Compares indexed key in AVL tree of 'data1' and 'data2'.
	 *
	 * Returns -1 if 'data1' < 'data2, 0 if 'data1' == 'data2',
	 * 1 if 'data1' > 'data2'.
	 */
	int (*keycmp)(void* data1, void* data2);

	/*
	 * Frees the provided memory pointed by data.
	 * Provided memory is a stored structure in the AVL tree.
	 */
	void (*freedata)(void* data);
};

/*
 * Initialize AVL data structure.
 *
 * 'keycmp' compares indexed key in AVL tree of 'data1' and 'data2'.
 * Must return -1 if data1 < data2, 0 if data1 == data2, 1 if data1 > data2.
 *
 * 'freedata' frees the provided memory pointed by data.
 * Provided memory is a structure stored in the AVL tree.
 * Used when freeing the AVL tree and stored data in avl_free().
 *
 * Success: returns a pointer to allocated memory for AVL tree.
 * Failure: returns NULL.
 */
struct avl_tree* avl_init(int (*keycmp)(void* data1, void* data2),
		void (*freedata)(void* data));

/*
 * Insert a value in the AVL tree.
 *
 * If a duplicate is inserted the insertion fails.
 *
 * Success: returns 1.
 * Failure: returns 0.
 */
int avl_insert(struct avl_tree* tree, void* value);

/*
 * Get the values in range ['min_val->key', 'max_val->key'].
 * The values are returned sorted.
 *
 * 'min_val' and 'max_val' are the type of data stored in the AVL tree.
 *
 * If 'min_val->key' or 'max_val->key' does not exist in AVL tree,
 * the next biggest and the next smallest respectively,
 * are the min and max value.
 *
 * 'tree' is the AVL tree to perform the search on.
 *
 * 'n_values' is the length of the result.
 *
 * Success: returns an array containing the filtered values.
 * Failure: returns NULL.
 */
void** avl_checkout_range(struct avl_tree* tree, void* min_val, void* max_val,
		unsigned* n_values);

/*
 * Convert AVL tree to min/max heap.
 *
 * Other AVL operation don't work after this conversion.
 * However, avl_pop() becomes available.
 *
 * 'heapify_keycmp': how to sort each node in the heap.
 * The return value of this function determines if the heap will be max or min.
 * For max-heap return -1 if data1 < data2, 0 if equal, 1 if data1 > data2.
 *
 * 'swap': swap the stored data in 'data1' and 'data2'.
 *
 * Always succeeds.
 */
void avl_heapify(struct avl_tree* tree, int (*heapify_keycmp)(void* data1, void* data2),
		void (*swap)(void* data1, void* data2));

/*
 * After executing avl_heapify(), you can call this function to extract the root.
 * You can run this function without previously calling avl_heapify(), but it
 * won't provide you with anything meaningful.
 *
 * 'tree': AVL tree to pop root.
 * Nodes are sorted as 'heapify_keycmp()' indicates.
 *
 * Returns the root data, NULL if tree is empty.
 *
 * Always succeeds.
 */
void* avl_pop(struct avl_tree* tree, int (*heapify_keycmp)(void* data1, void* data2));

/*
 * Free the memory allocated by the AVL tree.
 * 'tree' is the AVL tree to free.
 * Set 'free_data' to 1 for deleting stored data.
 * Set 'free_data' to 0 for deleting data structure only.
 *
 * Always succeeds.
 */
void avl_free(struct avl_tree* tree, int free_data);

#endif  // #ifndef AVL_H
