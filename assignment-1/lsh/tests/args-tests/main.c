#include <stdio.h>
#include <stdlib.h>

#include "args.h"

int main(void)
{
	printf("========== Test 1 - Start ==========\n");
	int argc1 = 5;
	char* argv1[] = { "prog", "-k", "100", "-L", "50" };
	struct args args1;
	// Set the defaults.
	args1 = args_defaults();
	// Tokenize argv.
	args1 = args_tokenize(&args1, argc1, argv1);
	// Print the `args` structure.
	args_print(args1);
	printf("========== Test 1 - Done ==========\n");

	printf("========== Test 2 - Start ==========\n");
	int argc2 = 5;
	char* argv2[] = { "prog", "-R", "50", "-N", "2" };
	struct args args2;
	// Set the defaults.
	args2 = args_defaults();
	// Tokenize argv.
	args2 = args_tokenize(&args2, argc2, argv2);
	// Print the `args` structure.
	args_print(args2);
	printf("========== Test 2 - Done ==========\n");

	printf("========== Test 3 - Start ==========\n");
	int argc3 = 1;
	char* argv3[] = { "prog" };
	struct args args3;
	// Set the defaults.
	args3 = args_defaults();
	// Tokenize argv.
	args3 = args_tokenize(&args3, argc3, argv3);
	// Print the `args` structure.
	args_print(args3);
	printf("========== Test 3 - Done ==========\n");

	printf("========== Test 4 - Start ==========\n");
	int argc4 = 5;
	char* argv4[] = { "prog", "k", "50", "-L", "10" };
	struct args args4;
	// Set the defaults.
	args4 = args_defaults();
	// Tokenize argv.
	args4 = args_tokenize(&args4, argc4, argv4);
	// Print the `args` structure.
	args_print(args4);
	printf("========== Test 4 - Done ==========\n");
}
