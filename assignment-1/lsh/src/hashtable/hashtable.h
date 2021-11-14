#ifndef HASHTABLE_H
#define HASHTABLE_H

struct hashtable {
	unsigned n_entries;     // Number of buckets.
	unsigned bucket_bytes;  // Size of each bucket in bytes.

	/*
	 * Compares the key of 'data' with key 'key'.
	 *
	 * 'data' is the type of data stored in the hashtable.
	 *
	 * Returns 0 if 'data->key' and 'key' are equal.
	 * Returns 1 if 'data->key' and 'key' are not equal.
	 */
	int (*keycmp)(void* data, void* key);

	/*
	 * Hashes the key 'key' at a bucket.
	 *
	 * 'key' is the key to hash.
	 * 'n_entries' is the number of buckets.
	 *
	 * Returns the bucket number.
	 */
	unsigned (*keyhash)(void* key, unsigned n_entries);

	/*
	 * Frees the data 'data'.
	 *
	 * 'data' is the data to free.
	 * It's the type of data stored in hashtable.
	 *
	 * Always succeeds.
	 */
	void (*freedata)(void* data);

	struct bucket* buckets[];  // Points to buckets.
};

/*
 * Allocates memory for hashtable and returns it.
 *
 * 'n_entries' is the number of buckets.
 * 'bucket_bytes' is the size of each bucket page in bytes.
 * 'keycmp' provides the way to compare a stored data with a key.
 * Must return 0 if keys are equal, 1 if not.
 * 'keyhash' provides the way to hash a key to a bucket.
 * 'freedata' provides the way to free a stored data.
 *
 * Success: returns pointer to hashtable struct.
 * Failure: returns NULL and sets errno accordingly.
 */
struct hashtable* ht_init(unsigned n_entries, unsigned bucket_bytes,
		int (*keycmp)(void* data, void* key),
		unsigned (keyhash)(void* key, unsigned n_entries),
		void (*freedata)(void* data));

/*
 * Inserts 'data' into hashtable 'hashtable'.
 *
 * 'hashtable' the hashtable to insert 'data' in.
 * 'data' the data to insert.
 * 'key' the key of 'data'.
 *
 * Success: returns 1.
 * Failure: returns 0.
 */
int ht_insert(struct hashtable* hashtable, void* data, void* key);

/*
 * Gets the entry in 'hashtable' with key equal to 'key'.
 *
 * 'hashtable' the hashtable to search in.
 * 'key' the key to search for.
 *
 * Success: returns pointer to the stored entry.
 * Failure: returns NULL entry does not exist.
 */
void* ht_checkout(struct hashtable* hashtable, void* key);

/*
 * Gets all entries in hashtable 'hashtable'.
 *
 * 'hashtable': Hashtable to search into.
 * 'n_values': Number of results (Number of pointers to stored data).
 *
 * Success: returns array of length 'n_values' with pointers to stored data.
 * Failure: returns NULL.
 */
void** ht_checkout_all(struct hashtable* hashtable, unsigned* n_values);

/*
 * Frees the hashtable's allocated memory.
 *
 * If 'free_data' is 1, the stored data are also freed
 * according to 'freedata' function.
 * If 'free_data' is 0, only the hashtable is freed.
 *
 * Always succeeds.
 */
void ht_free(struct hashtable* hashtable, int free_data);

#endif  // #ifndef HASHTABLE_H
