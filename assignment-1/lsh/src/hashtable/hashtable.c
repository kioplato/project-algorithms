#include <stdio.h>
#include <stdlib.h>
#include <errno.h>

#include "hashtable.h"

struct bucket {
	struct bucket* next;
	void* data[];
};

struct hashtable* ht_init(unsigned n_entries, unsigned bucket_bytes,
		int (*keycmp)(void* data, void* key),
		unsigned (*keyhash)(void* key, unsigned n_entries),
		void (*freedata)(void* data))
{
	struct hashtable* ht = NULL;
	size_t ht_mem = sizeof(struct hashtable) +
		sizeof(struct bucket*) * n_entries;

	if (n_entries == 0 || bucket_bytes == 0 ||
			keycmp == NULL || keyhash == NULL) {
		errno = EINVAL;
		return NULL;
	}

	if ((ht = malloc(ht_mem)) == NULL) return NULL;

	ht->n_entries = n_entries;
	ht->bucket_bytes = bucket_bytes;
	ht->keycmp = keycmp;
	ht->keyhash = keyhash;
	ht->freedata = freedata;
	
	// NULL each bucket pointer.
	for (unsigned i = 0; i < ht->n_entries; ht->buckets[i++] = NULL);

	return ht;
}

int ht_insert(struct hashtable* ht, void* data, void* key)
{
	// The bucket data hashed to.
	unsigned hashed_to = ht->keyhash(key, ht->n_entries);
	// Number of entries in a bucket.
	unsigned n_entries = (ht->bucket_bytes - sizeof(struct bucket)) /
								sizeof(void*);
	// Memory required for a new bucket.
	size_t bckt_mem = sizeof(struct bucket) + sizeof(void*) * n_entries;
	// The first free slot found in last bucket.
	unsigned free_slot;

	if (ht == NULL || data == NULL) return 0;

	struct bucket** bucket = &(ht->buckets[hashed_to]);
	if (*bucket == NULL) {
		*bucket = malloc(bckt_mem);
		(*bucket)->next = NULL;
		for (unsigned i = 0; i < n_entries; (*bucket)->data[i++] = NULL);
	}

	// Get to the last bucket.
	for (; (*bucket)->next != NULL; bucket = &((*bucket)->next));

	// Find the first free slot.
	for (free_slot = 0; free_slot < n_entries; free_slot++)
		if ((*bucket)->data[free_slot] == NULL) break;

	// The bucket is full.
	if (free_slot == n_entries) {
		(*bucket)->next = malloc(bckt_mem);
		bucket = &((*bucket)->next);
		free_slot = 0;
		for (unsigned i = 0; i < n_entries; (*bucket)->data[i++] = NULL);
		(*bucket)->next = NULL;
	}

	// Insert data.
	(*bucket)->data[free_slot] = data;

	return 1;
}

void* ht_checkout(struct hashtable* ht, void* key)
{
	// Number of entries in a bucket.
	unsigned n_entries = (ht->bucket_bytes - sizeof(struct bucket)) /
								sizeof(void*);
	// The bucket offset key hashed to.
	unsigned hashed_to = ht->keyhash(key, ht->n_entries);
	// For iterating bucket chain.
	struct bucket* bucket = ht->buckets[hashed_to];

	// Find the entry that does not diff with data[i]'s key.
	for (; bucket != NULL; bucket = bucket->next) {
		for (unsigned i = 0; i < n_entries && bucket->data[i] != NULL; ++i) {
			if (!ht->keycmp(bucket->data[i], key)) {
				return bucket->data[i];
			}
		}
	}

	return NULL;
}

void** ht_checkout_all(struct hashtable* ht, unsigned* n_values)
{
	if (ht->n_entries == 0) return NULL;

	unsigned c_value = 0;
	*n_values = 16;
	void** values = malloc(sizeof(void*) * (*n_values));

	unsigned n_entries = (ht->bucket_bytes - sizeof(struct bucket)) /
								sizeof(void*);
	
	struct bucket* bucket = NULL;
	for (unsigned i = 0; i < ht->n_entries; ++i) {
		bucket = ht->buckets[i];
		for (; bucket != NULL; bucket = bucket->next) {
			for (unsigned i = 0; i < n_entries; ++i) {
				if (bucket->data[i] == NULL) continue;
				if (c_value == *n_values) {
					*n_values *= 2;
					values = realloc(values, sizeof(void*) * (*n_values));
				}
				values[c_value++] = bucket->data[i];
			}
		}
	}

	if (c_value == 0) {
		free(values);
		values = NULL;
	}

	*n_values = c_value;
	return values;
}

void ht_free(struct hashtable* ht, int free_data)
{
	struct bucket* bucket = NULL;  // For free'ing bucket chains.
	struct bucket* prev_bucket = NULL;
	unsigned n_entries = (ht->bucket_bytes - sizeof(struct bucket)) /
								sizeof(void*);

	for (unsigned i = 0; i < ht->n_entries; ++i) {
		if (ht->buckets[i] != NULL) {
			for (bucket = ht->buckets[i]; bucket != NULL;) {
				if (free_data)
					for (unsigned i = 0; i < n_entries; ++i)
						if (bucket->data[i])
							ht->freedata(bucket->data[i]);
				prev_bucket = bucket;
				bucket = bucket->next;
				free(prev_bucket);
			}
		}
	}
	free(ht);
}
