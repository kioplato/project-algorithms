#include <stdio.h>
#include <stdlib.h>

#include <unistd.h>

#include "avl.h"

/*******************************************************************************
 * Helper structure for handling AVL functionality.
 ******************************************************************************/

struct avl_node {
	void* value;  // Actual data stored in node.

	struct avl_node* right;  // Right child of the node.
	struct avl_node* left;   // Left child of the node.

	int bf;  // Short for 'balance factor' of the node.
	unsigned height;  // The height of the node in the tree.
};

struct range_result {
	void** values;  // Array with the stored values.
	unsigned next_free;  // Next free index in 'values' array.
	unsigned n_slots;  // Total number of allocated slots.
};

/*******************************************************************************
 * Helper functions for handling AVL insertions.
 ******************************************************************************/

void update(struct avl_node* subroot)
{
	int left_height = -1;
	int right_height = -1;
	
	if (subroot->left != NULL) left_height = subroot->left->height;
	if (subroot->right != NULL) right_height = subroot->right->height;

	subroot->height = (left_height > right_height ?
					left_height : right_height) + 1;

	subroot->bf = right_height - left_height;
}

/*
 * Perform a right rotation on the provided AVL node.
 *
 * Success: returns the new subtree root.
 * Failure: returns NULL.
 */
struct avl_node* right_rotation(struct avl_node* subroot)
{
	struct avl_node* balanced_subroot = NULL;

	balanced_subroot = subroot->left;
	subroot->left = balanced_subroot->right;
	balanced_subroot->right = subroot;

	// Update the 'balance factor' and 'height'.
	update(subroot);
	update(balanced_subroot);

	return balanced_subroot;
}

/*
 * Perform a left rotation on the provided AVL node.
 *
 * Success: returns the new subtree root.
 * Failure: returns NULL.
 */
struct avl_node* left_rotation(struct avl_node* subroot)
{
	struct avl_node* balanced_subroot = NULL;

	balanced_subroot = subroot->right;
	subroot->right = balanced_subroot->left;
	balanced_subroot->left = subroot;

	// Update the 'balance factor' and 'height'.
	update(subroot);
	update(balanced_subroot);

	return balanced_subroot;
}

struct avl_node* balance(struct avl_node* subroot)
{
	// Left heavy subtree.
	if (subroot->bf == -2) {
		// Case left left.
		if (subroot->left->bf <= 0) return right_rotation(subroot);
		// Case left right.
		else {
			subroot->left = left_rotation(subroot->left);
			return right_rotation(subroot);
		}
	}
	// Right heavy subtree.
	else if (subroot->bf == 2) {
		// Case right right.
		if (subroot->right->bf >= 0) return left_rotation(subroot);
		// Case right left.
		else {
			subroot->right = right_rotation(subroot->right);
			return left_rotation(subroot);
		}
	}

	// Node has balance factor -1, 0 or 1.
	// Does not need balance.
	return subroot;
}

/*
 * Insert recursively 'value' in subtree with root 'subroot'.
 * 'tree is required for 'keycmp' function pointer.
 *
 * Success: returns the new subtree root.
 * Failure: returns NULL.
 */
struct avl_node* recursive_insert(struct avl_node* subroot, void* value,
		int (*keycmp)(void* data1, void* data2))
{
	if (subroot == NULL) {
		subroot = malloc(sizeof(struct avl_node));
		subroot->value = value;

		subroot->right = NULL;
		subroot->left = NULL;

		subroot->bf = 0;
		subroot->height = 0;

		return subroot;
	}

	if (keycmp(subroot->value, value) == 1)
		subroot->left = recursive_insert(subroot->left, value, keycmp);
	else if (keycmp(subroot->value, value) == -1)
		subroot->right = recursive_insert(subroot->right, value, keycmp);
	
	update(subroot);

	return balance(subroot);
}

int recursive_contains(struct avl_node* subroot, void* value,
		int (*keycmp)(void* data1, void* data2))
{
	if (subroot == NULL) return 0;

	if (keycmp(subroot->value, value) == -1)
		return recursive_contains(subroot->right, value, keycmp);
	if (keycmp(subroot->value, value) == 1)
		return recursive_contains(subroot->left, value, keycmp);

	return 1;
}

void recursive_free(struct avl_node* subroot, int free_data,
		void (*freedata)(void* data))
{
	if (subroot->left != NULL)
		recursive_free(subroot->left, free_data, freedata);
	if (subroot->right != NULL)
		recursive_free(subroot->right, free_data, freedata);

	if (free_data) freedata(subroot->value);

	free(subroot);
}

/*******************************************************************************
 * Implementations of interface functions declared in header file.
 ******************************************************************************/

struct avl_tree* avl_init(int (*keycmp)(void* data1, void* data2),
		void (*freedata)(void* data))
{
	struct avl_tree* tree = NULL;
	if ((tree = malloc(sizeof(struct avl_tree))) == NULL) return NULL;

	tree->root = NULL;
	tree->n_nodes = 0;

	tree->keycmp = keycmp;
	tree->freedata = freedata;

	return tree;
}

int avl_insert(struct avl_tree* tree, void* value)
{
	if (value == NULL) return 0;

	if (!recursive_contains(tree->root, value, tree->keycmp)) {
		tree->root = recursive_insert(tree->root, value, tree->keycmp);
		tree->n_nodes++;
		return 1;
	}

	return 0;
}

// TODO: move to private functions.
void recursive_checkout_range(struct avl_node* subroot, void* min_val,
		void* max_val, struct range_result* result,
		int (*keycmp)(void* data1, void* data2))
{
	if (subroot == NULL) return;

	// Current node's data is smaller than 'min_val'.
	if (keycmp(subroot->value, min_val) == -1) {
		recursive_checkout_range(subroot->right, min_val, max_val,
				result, keycmp);
		return;
	}
	// Current node's data is bigger than 'max_val'.
	if (keycmp(subroot->value, max_val) == 1) {
		recursive_checkout_range(subroot->left, min_val, max_val,
				result, keycmp);
		return;
	}

	if (subroot->left != NULL)
		recursive_checkout_range(subroot->left, min_val, max_val,
				result, keycmp);

	// Check for space in result to add current node's data.
	if (result->next_free == result->n_slots) {
		result->n_slots *= 2;
		result->values =
			realloc(result->values, sizeof(void*) * result->n_slots);
	}
	// Add current node's data to result.
	result->values[result->next_free] = subroot->value;
	result->next_free++;

	if (subroot->right != NULL)
		recursive_checkout_range(subroot->right, min_val, max_val,
				result, keycmp);
}

void** avl_checkout_range(struct avl_tree* tree, void* min_val, void* max_val,
		unsigned* n_values)
{
	struct range_result* result = malloc(sizeof(struct range_result));
	unsigned n_slots = 8;

	result->values = malloc(sizeof(void*) * n_slots);
	result->next_free = 0;
	result->n_slots = n_slots;

	recursive_checkout_range(tree->root, min_val, max_val, result,
			tree->keycmp);

	// The result is empty.
	if (result->next_free == 0) {
		free(result->values);
		result->values = NULL;
	}
	*n_values = result->next_free;
	void** values = result->values;
	free(result);
	return values;
}

void recursive_heapify(struct avl_node* subroot,
		int (*heapify_keycmp)(void* data1, void* data2),
		void (*swap)(void* data1, void* data2))
{
	if (subroot->left)
		recursive_heapify(subroot->left, heapify_keycmp, swap);
	if (subroot->right)
		recursive_heapify(subroot->right, heapify_keycmp, swap);

	struct avl_node* max_node = subroot;
	if (subroot->left)
		if (heapify_keycmp(max_node->value, subroot->left->value) == -1)
			max_node = subroot->left;
	if (subroot->right)
		if (heapify_keycmp(max_node->value, subroot->right->value) == -1)
			max_node = subroot->right;

	if (max_node != subroot) {
		swap(subroot->value, max_node->value);
		recursive_heapify(max_node, heapify_keycmp, swap);
	}
}

void avl_heapify(struct avl_tree* tree, int (*heapify_keycmp)(void* data1, void* data2),
		void (*swap)(void* data1, void* data2))
{
	if (tree->root != NULL)
		recursive_heapify(tree->root, heapify_keycmp, swap);
}

void heapify_node(struct avl_node* subroot, int (*heapify_keycmp)(void* data1, void* data2))
{
	struct avl_node* max_node = subroot;
	if (subroot->left)
		if (heapify_keycmp(max_node->value, subroot->left->value) == -1)
			max_node = subroot->left;
	if (subroot->right)
		if (heapify_keycmp(max_node->value, subroot->right->value) == -1)
			max_node = subroot->right;

	void* temp = max_node->value;
	max_node->value = subroot->value;
	subroot->value = temp;

	if (max_node != subroot) heapify_node(max_node, heapify_keycmp);
}

struct avl_node* extract_deepest(struct avl_node* subroot)
{
	struct avl_node* deepest_node = NULL;
	if (subroot->height == 0) deepest_node = subroot;
	else if (subroot->height == 1) {
		if (subroot->bf < 0) { deepest_node = subroot->left; subroot->left = NULL; }
		else { deepest_node = subroot->right; subroot->right = NULL; }
	}
	else if (subroot->bf < 0) deepest_node = extract_deepest(subroot->left);
	else deepest_node = extract_deepest(subroot->right);

	update(subroot);

	return deepest_node;
}

void* avl_pop(struct avl_tree* tree, int (*heapify_keycmp)(void* data1, void* data2))
{
	if (tree->root == NULL) return NULL;

	void* value = tree->root->value;

	struct avl_node* deepest_node = extract_deepest(tree->root);
	tree->root->value = deepest_node->value;
	free(deepest_node);

	if (tree->root == deepest_node) tree->root = NULL;

	if (tree->root) heapify_node(tree->root, heapify_keycmp);

	return value;
}

void avl_free(struct avl_tree* tree, int free_data)
{
	if (tree->root != NULL)
		recursive_free(tree->root, free_data, tree->freedata);
	free(tree);
}
