#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "fparser.h"

int f_errno = FPARSE_OK;

void* ftoken(FILE* stream, void* delims)
{
	f_errno = FPARSE_OK;

	if (stream == NULL || delims == NULL) {
		f_errno = FPARSE_INV_PARAMS;
		return NULL;
	}

	unsigned n_delims = *(unsigned*)delims;
	delims += sizeof(unsigned);

	unsigned n_bytes = 16;  // Number of bytes allocated.
	unsigned c_bytes = 0;   // Number of bytes occupied by read bytes.
	void* mem = malloc(n_bytes + sizeof(unsigned));
	void* line = mem + sizeof(unsigned);

	while (fread(line + c_bytes, 1, 1, stream) != 0) {
		if (feof(stream) || ferror(stream)) goto exit_out;

		++c_bytes;

		for (unsigned i = 0; i < n_delims; ++i)
			if (!memcmp(delims + i, line + c_bytes - 1, 1))
				goto exit_out;

		if (c_bytes == n_bytes) {
			mem = realloc(mem, (n_bytes *= 2) + sizeof(unsigned));
			if (mem == NULL) { perror("malloc"); exit(1); }
			line = mem + sizeof(unsigned);
		}
	}

exit_out:
	if (feof(stream)) f_errno = FPARSE_FEOF;
	else if (ferror(stream)) f_errno = FPARSE_FERROR;

	// Nothing was read.
	if (c_bytes == 0) { free(mem); mem = NULL; }
	else *(unsigned*)mem = c_bytes;

	return mem;
}

extern void* convert_NULL(const void*, void* token)
{
	unsigned token_bytes = *(unsigned*)token + sizeof(unsigned);
	return memcpy(malloc(token_bytes), token, token_bytes);
}

void* fparse(FILE* stream, void* (*convert)(const void**, void*), void* delims)
{
	if (!stream || !delims) { f_errno = FPARSE_INV_PARAMS; return NULL; }

	f_errno = FPARSE_OK;  // Redundant since ftoken() will set f_errno.

	if (!convert) convert = convert_NULL;

	unsigned n_convs = 16;  // void* pointers for storing converted tokens.
	unsigned c_conv = 0;    // Occupied pointers from converted tokens.
	unsigned offset = sizeof(unsigned);
	void* mem = malloc(offset + n_convs * sizeof(void*));
	void** convs = mem + offset;
	*(unsigned*)mem = 0;

	void* token = NULL;
	while ((token = ftoken(stream, delims)) != NULL) {
		void* conv = convert((const void**)mem, token);
		if (conv == NULL) { free(token); continue; }

		convs[c_conv++] = conv;

		if (c_conv == n_convs) {
			mem = realloc(mem, offset + (n_convs *= 2) * sizeof(void*));
			if (mem == NULL) { perror("malloc"); exit(1); }
			convs = mem + offset;
		}

		++*(unsigned*)mem;  // Convert needs to know the tokens.

		free(token);
	}

	if (c_conv == 0) { free(mem); mem = NULL; }

	// f_errno is set by ftoken() at this point.
	return mem;
}

void* nldelim()
{
	void* delims = malloc(sizeof(unsigned) + sizeof(char));
	*(unsigned*)delims = 1;
	*(char*)(delims + sizeof(unsigned)) = '\n';

	return delims;
}
