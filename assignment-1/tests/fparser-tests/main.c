#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>

#include "fparser/fparser.h"
#include "fparser/converts.h"
#include "errors/errors.h"

void ftoken_test1()
{
	printf("========== [ftoken] Test 1 - Start - Read text file line by line ==========\n");

	char* filename = "example-inputs/example-valid-file.txt";

	printf("Opening file %s for reading.\n", filename);
	FILE* fp = fopen(filename, "r");

	void* delims = malloc(sizeof(unsigned) + sizeof(char));
	*(unsigned*)delims = 1;
	*(char*)(delims + sizeof(unsigned)) = '\n';

	printf("Reading whole file.\n");
	char* mem = NULL;
	while ((mem = (char*)ftoken(fp, delims)) != NULL) {
		unsigned bytes = *(unsigned*)mem;
		char* line = malloc(bytes + 1);
		memcpy(line, mem + sizeof(unsigned), bytes);
		*(line + bytes) = 0;
		free(mem);

		printf("Printing read line:\n");
		printf("%s", line);
		printf("line's length = %u\n", strlen(line));
		printf("mem's bytes = %u\n", bytes);
		printf("f_errno = %d\n", f_errno);

		free(line);
	}

	printf("f_errno = %d\n", f_errno);

	free(delims);
	fclose(fp);

	printf("========== [ftoken] Test 1 - Done ==========\n");
}

void ftoken_test2()
{
	printf("========== [ftoken] Test 2 - Start - Read text file until EOF by not specifying delims ==========\n");

	char* filename = "example-inputs/example-valid-file.txt";

	printf("Opening file %s for reading.\n", filename);
	FILE* fp = fopen(filename, "r");

	void* delims = malloc(sizeof(unsigned));
	*(unsigned*)delims = 0;

	printf("Reading whole file.\n");
	char* mem = NULL;
	while ((mem = (char*)ftoken(fp, delims)) != NULL) {
		unsigned bytes = *(unsigned*)mem;
		char* line = malloc(bytes + 1);
		memcpy(line, mem + sizeof(unsigned), bytes);
		*(line + bytes) = 0;
		free(mem);

		printf("Printing read line:\n");
		printf("%s", line);
		printf("line's length = %u\n", strlen(line));
		printf("mem's bytes = %u\n", bytes);
		printf("f_errno = %d\n", f_errno);

		free(line);
	}

	printf("f_errno = %d\n", f_errno);

	free(delims);
	fclose(fp);

	printf("========== [ftoken] Test 2 - Done ==========\n");
}

void ftoken_test3()
{
	printf("========== [ftoken] Test 3 - Start - Read text file line by line, but close it midway ==========\n");

	char* filename = "example-inputs/example-valid-file.txt";

	printf("Opening file %s for reading.\n", filename);
	FILE* fp = fopen(filename, "r");

	void* delims = malloc(sizeof(unsigned) + sizeof(char));
	*(unsigned*)delims = 1;
	*(char*)(delims + sizeof(unsigned)) = '\n';

	unsigned read_lines = 0;

	printf("Reading whole file.\n");
	char* mem = NULL;
	while ((mem = (char*)ftoken(fp, delims)) != NULL) {
		unsigned bytes = *(unsigned*)mem;
		char* line = malloc(bytes + 1);
		memcpy(line, mem + sizeof(unsigned), bytes);
		*(line + bytes) = 0;
		free(mem);

		printf("Printing read line:\n");
		printf("%s", line);
		printf("line's length = %u\n", strlen(line));
		printf("mem's bytes = %u\n", bytes);
		printf("f_errno = %d\n", f_errno);

		free(line);

		if (++read_lines == 2) fclose(fp);
	}

	printf("f_errno = %d\n", f_errno);
	printf("errno = %d\n", errno);
	printf("Error: %s\n", strerror(errno));

	free(delims);

	printf("========== [ftoken] Test 3 - Done ==========\n");
}

void ftoken_test4()
{
	printf("========== [ftoken] Test 4 - Start - Read text file line by line ==========\n");

	char* filename = "example-inputs/sample-large.txt";

	printf("Opening file %s for reading.\n", filename);
	FILE* fp = fopen(filename, "r");

	void* delims = malloc(sizeof(unsigned) + sizeof(char));
	*(unsigned*)delims = 1;
	*(char*)(delims + sizeof(unsigned)) = '\n';

	printf("Reading whole file.\n");
	char* mem = NULL;
	while ((mem = (char*)ftoken(fp, delims)) != NULL) {
		unsigned bytes = *(unsigned*)mem;
		char* line = malloc(bytes + 1);
		memcpy(line, mem + sizeof(unsigned), bytes);
		*(line + bytes) = 0;
		free(mem);

		printf("Printing read line:\n");
		printf("%s", line);
		printf("line's length = %u\n", strlen(line));
		printf("mem's bytes = %u\n", bytes);
		printf("f_errno = %d\n", f_errno);

		free(line);
	}

	printf("f_errno = %d\n", f_errno);

	free(delims);
	fclose(fp);

	printf("========== [ftoken] Test 1 - Done ==========\n");
}

void* convert_test1(const void*, void* token)
{
	unsigned token_len = *(unsigned*)token;
	char* conv = malloc(token_len + 1);
	memcpy(conv, token + sizeof(unsigned), token_len);
	conv[token_len] = '\0';

	return conv;
}

void fparse_test1()
{
	printf("========== [fparse] Test 1 - Start - Convert tokens to strings ==========\n");

	char* filename = "example-inputs/example-valid-file.txt";

	printf("Opening file %s for reading.\n", filename);
	FILE* fp = fopen(filename, "r");

	printf("Setting up delims.\n");
	void* delims = malloc(sizeof(unsigned) + sizeof(char));
	*(unsigned*)delims = 1;
	*(char*)(delims + sizeof(unsigned)) = '\n';

	printf("fparse'ing.\n");
	void* mem = fparse(fp, &convert_test1, delims);
	printf("f_errno = %d\n", f_errno);
	if (mem == NULL) { printf("Error: NULL mem\n"); exit(1); }

	unsigned n_strs = *(unsigned*)mem;
	char** strs = mem + sizeof(unsigned);
	printf("fparse'd %u convs.\n", n_strs);

	printf("Printing the fparse convs.\n");
	for (unsigned c_str = 0; c_str < n_strs; ++c_str)
	{ printf("String %u: \"%s\"\n", c_str, strs[c_str]); free(strs[c_str]); }
	free(mem);

	free(delims);
	fclose(fp);

	printf("========== [fparse] Test 1 - Done ==========\n");
}

void fparse_test2()
{
	printf("========== [fparse] Test 2 - Start - NULL convert ==========\n");

	char* filename = "example-inputs/example-valid-file.txt";

	printf("Opening file %s for reading.\n", filename);
	FILE* fp = fopen(filename, "r");

	printf("Setting up delims.\n");
	void* delims = malloc(sizeof(unsigned) + sizeof(char));
	*(unsigned*)delims = 1;
	*(char*)(delims + sizeof(unsigned)) = '\n';

	printf("fparse'ing.\n");
	void* mem = fparse(fp, NULL, delims);
	printf("f_errno = %d\n", f_errno);

	unsigned n_convs = *(unsigned*)mem;
	void** convs = mem + sizeof(unsigned);
	printf("fparse'd %u convs.\n", n_convs);

	printf("Printing the fparse convs.\n");
	for (unsigned c_conv = 0; c_conv < n_convs; ++c_conv) {
		unsigned conv_len = *(unsigned*)(convs[c_conv]);
		char* conv = convs[c_conv] + sizeof(unsigned);
		printf("Conv has %u length.\n", conv_len);
		printf("\"");
		for (unsigned c_byte = 0; c_byte < conv_len; ++c_byte)
			printf("%c", conv[c_byte]);
		printf("\"\n");

		free(convs[c_conv]);
	}
	free(mem);

	free(delims);
	fclose(fp);

	printf("========== [fparse] Test 2 - Done ==========\n");
}

void fparse_test3()
{
	printf("========== [fparse] Test 3 - Start - NULL stream ==========\n");

	FILE* fp = NULL;

	printf("Setting up delims.\n");
	void* delims = malloc(sizeof(unsigned) + sizeof(char));
	*(unsigned*)delims = 1;
	*(char*)(delims + sizeof(unsigned)) = '\n';

	printf("fparse'ing.\n");
	void* mem = fparse(fp, NULL, delims);
	printf("f_errno = %d\n", f_errno);
	printf("mem is NULL: %d\n", mem == NULL);

	free(delims);

	printf("========== [fparse] Test 3 - Done ==========\n");
}

// Copies and returns token. Prints mem to stdout as chars.
void* convert_test4(const void* mem, void* token)
{
	// Print mem's convs.
	unsigned n_convs = *(unsigned*)mem;
	void* const * convs = mem + sizeof(unsigned);
	printf("Printing %u convs.\n", n_convs);
	for (unsigned c_conv = 0; c_conv < n_convs; ++c_conv) {
		const char* conv = convs[c_conv] + sizeof(unsigned);
		unsigned conv_len = *((unsigned*)convs[c_conv]);
		printf("Printing %u conv with length %u: \"", c_conv, conv_len);
		for (unsigned c_byte = 0; c_byte < conv_len; ++c_byte)
			printf("%c", conv[c_byte]);
		printf("\"\n");
	}

	unsigned token_bytes = *(unsigned*)token + sizeof(unsigned);
	return memcpy(malloc(token_bytes), token, token_bytes);
}

void fparse_test4()
{
	printf("========== [fparse] Test 4 - Start - Copy and return token, and print mem in convert ==========\n");

	char* filename = "example-inputs/example-valid-file.txt";

	printf("Opening file %s for reading.\n", filename);
	FILE* fp = fopen(filename, "r");

	printf("Setting up delims.\n");
	void* delims = malloc(sizeof(unsigned) + sizeof(char));
	*(unsigned*)delims = 1;
	*(char*)(delims + sizeof(unsigned)) = '\n';

	printf("fparse'ing.\n");
	void* mem = fparse(fp, &convert_test4, delims);
	printf("f_errno = %d\n", f_errno);

	unsigned n_convs = *(unsigned*)mem;
	void** convs = mem + sizeof(unsigned);
	printf("fparse'd %u convs.\n", n_convs);

	printf("Printing the fparse'd convs.\n");
	for (unsigned c_conv = 0; c_conv < n_convs; ++c_conv) {
		char* conv = convs[c_conv] + sizeof(unsigned);
		unsigned conv_len = *((unsigned*)(convs[c_conv]));
		printf(">>>>> conv_len = %u\n", conv_len);
		printf("Printing %u conv with length %u: \"", c_conv, conv_len);
		for (unsigned c_byte = 0; c_byte < conv_len; ++c_byte)
			printf("%c", conv[c_byte]);
		printf("\"\n");

		free(convs[c_conv]);
	}
	free(mem);

	free(delims);
	fclose(fp);

	printf("========== [fparse] Test 4 - Done ==========\n");
}

void fparse_test5()
{
	printf("========== [fparse] Test 5 - Start - Convert tokens to strings (Large input file) ==========\n");

	char* filename = "example-inputs/sample-large.txt";

	printf("Opening file %s for reading.\n", filename);
	FILE* fp = fopen(filename, "r");

	printf("Setting up delims.\n");
	void* delims = malloc(sizeof(unsigned) + sizeof(char));
	*(unsigned*)delims = 1;
	*(char*)(delims + sizeof(unsigned)) = '\n';

	printf("fparse'ing.\n");
	void* mem = fparse(fp, &convert_test1, delims);
	printf("f_errno = %d\n", f_errno);

	unsigned n_strs = *(unsigned*)mem;
	char** strs = mem + sizeof(unsigned);
	printf("fparse'd %u convs.\n", n_strs);

	printf("Printing the fparse convs.\n");
	for (unsigned c_str = 0; c_str < n_strs; ++c_str) {
		printf("String %u: \"%s\"\n", c_str, strs[c_str]);
		free(strs[c_str]);
	}
	free(mem);

	free(delims);
	fclose(fp);

	printf("========== [fparse] Test 5 - Done ==========\n");
}

void fparse_test6()
{
	printf("========== [fparse] Test 6 - Start - Convert tokens to vectors ==========\n");

	char* filename = "example-inputs/example-vectors.txt";

	printf("Opening file %s for reading.\n", filename);
	FILE* fp = fopen(filename, "r");

	printf("Setting up delims.\n");
	void* delim = nldelim();

	printf("fparse'ing.\n");
	void* mem = fparse(fp, &convert_to_vector, delim);
	printf("f_errno = %d\n", f_errno);

	free(delim);
	fclose(fp);

	unsigned n_vectors = *(unsigned*)mem;
	struct vector** vectors = mem + sizeof(unsigned);
	printf("fparse'd %u tokens.\n", n_vectors);

	printf("Printing the fparse'd tokens.\n");
	for (unsigned c_vector = 0; c_vector < n_vectors; ++c_vector) {
		printf(">>>>> Printing %u vector.\n", c_vector);
		vector_print(vectors[c_vector]);
		vector_free(vectors[c_vector]);
	}
	free(mem);

	printf("========== [fparse] Test 6 - Done ==========\n");
}

void fparse_test7()
{
	printf("========== [fparse] Test 7 - Start - Convert tokens to vectors ==========\n");

	char* filename = "example-inputs/sample-big-vectors.txt";

	printf("Opening file %s for reading.\n", filename);
	FILE* fp = fopen(filename, "r");

	printf("Setting up delims.\n");
	void* delim = nldelim();

	printf("fparse'ing.\n");
	void* mem = fparse(fp, &convert_to_vector, delim);
	printf("f_errno = %d\n", f_errno);

	free(delim);
	fclose(fp);

	unsigned n_vectors = *(unsigned*)mem;
	struct vector** vectors = mem + sizeof(unsigned);
	printf("fparse'd %u tokens.\n", n_vectors);

	printf("Printing the fparse'd tokens.\n");
	for (unsigned c_vector = 0; c_vector < n_vectors; ++c_vector) {
		printf("%u ", vectors[c_vector]->id);
		unsigned n_dims = vectors[c_vector]->n_dims;
		for (unsigned c_dim = 0; c_dim < n_dims; ++c_dim)
			printf("%u ", vectors[c_vector]->dims[c_dim]);
		printf("\r\n");  // So that `diff` comes out clean.

		vector_free(vectors[c_vector]);
	}
	free(mem);

	printf("========== [fparse] Test 7 - Done ==========\n");
}

int main(void)
{
	printf("********** ftoken TESTS START **********\n");
	// File line by line.
	ftoken_test1();

	// Whole file.
	ftoken_test2();

	// Read from closed stream. The behaviour is undefined.
	//ftoken_test3();

	// Same as `ftoken_test1()` but using a large input file.
	ftoken_test4();
	printf("********** ftoken TESTS DONE **********\n");

	printf("********** fparse TESTS START **********\n");
	// Convert tokens to strings. Print them.
	fparse_test1();

	// NULL convert.
	fparse_test2();

	// NULL stream passed.
	fparse_test3();

	// In convert copy and return token, and print mem.
	fparse_test4();

	// Same as `fparse_test1()` but using a large file.
	fparse_test5();

	// Convert tokens to vectors.
	fparse_test6();

	// Convert tokens to vectors (Large file).
	fparse_test7();
	printf("********** fparse TESTS DONE **********\n");

	return EXIT_SUCCESS;
}
